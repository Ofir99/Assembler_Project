#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simulator.h"

//command line aplication:sim.exe memin.txt diskin.txt irq2in.txt memout.txt regout.txt trace.txt hwregtrace.txt cycles.txt leds.txt display.txt diskout.txt
int main(int argc, char* argv[]) {
	FILE* memin = NULL, *diskin = NULL, *irq2in = NULL, *memout = NULL, *regout = NULL, *trace = NULL, *hwregtrace = NULL, *cycles = NULL, *leds = NULL, *display = NULL, *diskout = NULL;
	memin = fopen(argv[1], "r");
	diskin = fopen(argv[2], "r");
	irq2in = fopen(argv[3], "r");
	memout = fopen(argv[4], "w+");
	regout = fopen(argv[5], "w");
	trace = fopen(argv[6], "w");
	hwregtrace = fopen(argv[7], "w");
	cycles = fopen(argv[8], "w");
	leds = fopen(argv[9], "w");
	display = fopen(argv[10], "w");
	diskout = fopen(argv[11], "w+");

	if ((memin==NULL) || (diskin==NULL) || (irq2in==NULL) || (memout==NULL) || (regout==NULL) || (trace==NULL) || (hwregtrace==NULL) || (cycles==NULL) || (leds==NULL) || (display==NULL) || (diskout == NULL)) {
		printf("can't open one of the files\n");
		return 1;
	}
	Copy_Text_File(memin,memout);
	Copy_Text_File(diskin, diskout);
	adding_zeros_rows(diskout);
	Simulator(memout, trace, leds, diskout, hwregtrace, regout, cycles, display);

	fclose(memin);
	fclose(diskin);
	fclose(irq2in);
	fclose(memout);
	fclose(regout);
	fclose(trace);
	fclose(hwregtrace);
	fclose(cycles);
	fclose(leds);
	fclose(display);
	fclose(diskout);
	return 0;
}


//this function simulates a SIMP processor 
//input argument: file that created in assembler function
void Simulator(FILE* Memout,FILE *trace, FILE *leds, FILE *diskout,FILE *hwregtrace, FILE *regout, FILE *cycles, FILE *display) {//need to add the rest of the files
	int R[MAX_REG] = { 0 };
	int opcode = 0;
	int	rd = 0;
	int rs = 0;
	int rt = 0;
	int imm = 0;
	int PC = 0;
	int PC_next = 0;
	int IORegister[MAX_IOREG] = { 0 };
	int Clock_Cycle = 0;

	while (1) //loop until halt opcode  
	{
		PC = PC_next;
		PC_next = PC + 1;
		
		Extract_Variabales_from_PC(Memout, PC, &opcode, &rd, &rs, &rt, &imm);
		R[$IMM] = imm;//update $imm register
		print_trace(trace, PC, opcode, rd, rs, rt, imm, R);

		if (opcode >= ADD && opcode <= JAL) Instructions_0_to_13_opcode(R, opcode, rd, rs, rt, PC, &PC_next);
		if (opcode >= LW && opcode <= SW) Instructions_lw_sw(R, opcode, rd, rs, rt, PC, &PC_next, Memout); 
		if (opcode >= RETI && opcode <= OUT) IO_Instructions(hwregtrace,leds,diskout, Memout, opcode,R,IORegister, rd,rs,rt, &PC_next, Clock_Cycle);
		if (opcode == HALT) break;//exit program

		Clock_Cycle++;
	}
	fprintf(cycles, "%d\n", Clock_Cycle);//printing to cycles files the number of clock cycles
	print_regout(regout, R);
	return 0;
}


//this function move the pointer in file to PC address
void Jump_to_PC(FILE* f, int PC) {
	fseek(f, PC * 10, SEEK_SET);//10 bytes in line 
}


//the function allocates data to variabales from the PC adress
//input arg1: pointer to file , input arg2: number of line to read from(PC), input arg3,4,5,6,7:poniter to variabels
void Extract_Variabales_from_PC(FILE* f, int  PC, int* opcode, int* rd, int* rs, int* rt, int* imm) {
	Jump_to_PC(f, PC);
	fscanf(f, "%02X%01X%01X%01X%03X\n", opcode, rd, rs, rt, imm);//extracting values 
	*imm = SIGNED_EXT_IMM(*imm);//return imm with sign extension to 32 bits
	return;
}

void Instructions_0_to_13_opcode(int R[], int opcode, int rd, int rs, int rt, int PC, int* PC_next) {
	switch (opcode)
	{
	case ADD:
		R[rd] = R[rs] + R[rt];
		break;
	case SUB:
		R[rd] = R[rs] - R[rt];
		break;
	case AND:
		R[rd] = R[rs] & R[rt];
		break;
	case OR:
		R[rd] = R[rs] | R[rt];
		break;
	case SLL:
		R[rd] = R[rs] << R[rt];
		break;
	case SRA: //keep sign
		R[rd] = R[rs] >> R[rt];
		break;
	case SRL: //not keep sign
		R[rd] = (unsigned int)R[rs] >> R[rt];
		break;
	case BEQ:
		if (R[rs] == R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case BNE:
		if (R[rs] != R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case BLT:
		if (R[rs] < R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case BGT:
		if (R[rs] > R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case BLE:
		if (R[rs] <= R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case BGE:
		if (R[rs] >= R[rt]) *PC_next = MASK_REG(R[rd]);
		break;
	case JAL:
		R[15] = PC + 1;
		*PC_next = MASK_REG(R[rd]);
		break;

	}
}

void Instructions_lw_sw(int R[], int opcode, int rd, int rs, int rt, int PC, int* PC_next, FILE* Memout)
{
	switch (opcode) {
	case LW:
	{
		Jump_to_PC(Memout, R[rs] + R[rt]);
		fscanf(Memout, "%X\n", &R[rd]); //נופל כאן בריצה, אולי לעשות עם מערך...? את כל הקובץ
		break;
	}
	case SW: //לבדוק גם את זה ופעולות נוספות שלא בדקנו
	{
		Jump_to_PC(Memout, R[rs] + R[rt]);
		fprintf(Memout, "%X\n", R[rd]);
		break;
	}
	}
}
void IO_Instructions(FILE* hwregtrace,FILE* leds, FILE* diskout,FILE* memout,int opcode, int R[], int IORegister[], int rd, int rs, int rt, int* PC_next, int clock_cycle) {
	int prev_leds = IORegister[LEDS];
	char IOR_name[MAX_IOREG][13] = { "irq0enable"   ,"irq1enable"  ,"irq2enable"  ,\
									 "irq0status"   ,"irq1status"  ,"irq2status"  ,\
									 "irqhandler"   ,"irqreturn"   ,"clks"        ,\
									 "leds"		    , "display"    ,"timerenable" ,\
									 "timercurrent" ,"timermax"    ,"diskcmd"     ,\
									 "disksector"   ,"diskbuffer"  ,"diskstatus" };

	switch (opcode) {
	case RETI:
		*PC_next = IORegister[IRQRETURN];
		break;

	case IN:
		R[rd] = IORegister[R[rs] + R[rt]];
		fprintf(hwregtrace, "%d READ %s %08x\n", clock_cycle, (IOR_name + R[rs] + R[rt]), R[rd]);//printing to file reading to registers operation
		break;

	case OUT:
		IORegister[R[rs] + R[rt]] = R[rd];
		fprintf(hwregtrace, "%d WRITE %s %08x\n", clock_cycle, (IOR_name + R[rs] + R[rt]), R[rd]);//printing to file writing to registers operation

		if (IORegister[LEDS] != prev_leds) {//if leds register changed, write to leds file
			print_leds(leds,clock_cycle, IORegister[LEDS]);
		}
		if ((IORegister[DISKSTATUS] ==0)&& IORegister[DISKCMD] != 0){//checking if disk is free and if its read/write command to disk
		read_write_to_disk(diskout, memout, IORegister[DISKCMD], IORegister[DISKSECTOR], IORegister[DISKBUFFER]);
		IORegister[DISKSTATUS] = 1;//disk is busy
		}
		
		break;
	}
}
//duplicate source file to target file
void Copy_Text_File(FILE* source, FILE* target) {
	char ch;
	while ((ch = fgetc(source)) != EOF)
		fputc(ch, target);
}
//printting to trace file each clock cycle
void print_trace(FILE* trace,int PC,int opcode,int rd,int rs,int rt,int imm,int R[]) {

	fprintf(trace, "%08X ", PC);
	fprintf(trace, "%02X%01X%01X%01X%03X ", opcode, rd, rs, rt, MASK_REG(imm));

	for (int i = 0; i < MAX_REG; i++)//printing registers
		fprintf(trace, "%08X ", R[i]);
	fprintf(trace, "\n");
}
//printing to leds file evertime there is a change in register leds
void print_leds(FILE *leds, int clock_cycle, int new_leds) {
	fprintf(leds, "%d %08X\n", clock_cycle, new_leds);
}

void print_regout(FILE *regout,int R[]) {
	int i = 2;//

	for (i = 2; i < MAX_REG; i++) //printing R2-R15
		fprintf(regout, "%08X\n", R[i]);
}