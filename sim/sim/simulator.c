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
	Simulator(memout, trace, leds, diskout, hwregtrace, regout, cycles, display, irq2in);

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
//input argument 1: file that created in assembler function
void Simulator(FILE* Memout,FILE *trace, FILE *leds, FILE *diskout,FILE *hwregtrace, FILE *regout, FILE *cycles, FILE *display, FILE *irq2in) {
	int R[MAX_REG] = { 0 };
	int opcode = 0;
	int	rd = 0;
	int rs = 0;
	int num = 0;
	int rt = 0;
	int imm = 0;
	int PC = 0;
	int PC_next = 0;
	int p = 0; // p=1 if we handling interupt 
	int IORegister[MAX_IOREG] = { 0 };
	unsigned int Clock_Cycle = 0;
	int timerdisk = 0;

	while (1) //loop until halt opcode  
	{
		
		if (PC_next == IORegister[IRQRETURN])
			p = 0;
		if (((IORegister[IRQ0ENABLE] && IORegister[IRQ0STATUS])|| (IORegister[IRQ1ENABLE] && IORegister[IRQ1STATUS]) || (IORegister[IRQ2ENABLE] && IORegister[IRQ2STATUS])) && p==0)
		{
			IORegister[IRQRETURN] = PC_next;
			PC_next = IORegister[IRQHANDLER];
			p = 1;
		}
		routine_file(irq2in, IORegister, Clock_Cycle, &num);
		routine_timer(IORegister);
		routine_disk(IORegister, &timerdisk);
		
		IORegister[CLKS] = Clock_Cycle;
		PC = PC_next;
		PC_next = PC + 1;
		
		Extract_Variabales_from_PC(Memout, PC, &opcode, &rd, &rs, &rt, &imm);
		R[$IMM] = imm;//update $imm register
		print_trace(trace, PC, opcode, rd, rs, rt, imm, R);

		if (opcode >= ADD && opcode <= JAL) Instructions_0_to_13_opcode(R, opcode, rd, rs, rt, PC, &PC_next);
		if (opcode >= LW && opcode <= SW) Instructions_lw_sw(R, opcode, rd, rs, rt, PC, &PC_next, Memout); 
		if (opcode >= RETI && opcode <= OUT) IO_Instructions(hwregtrace,leds,diskout, Memout,display, opcode,R,IORegister, rd,rs,rt, &PC_next, Clock_Cycle);
		if (opcode == HALT) break;//exit program

		Clock_Cycle++;
	}
	fprintf(cycles, "%d\n", Clock_Cycle+1);//printing to cycles file the number of clock cycles
	print_regout(regout, R);//printing R2-R15 
	return;
}

// input arg: the IOregister array
// output result: each clock cycle the function promoting the relevant registers related to the timer, if timer is using.
void routine_timer(int IORregister[])
{
	
	if (IORregister[TIMERENABLE]==1)
	{
		if (IORregister[TIMERCURRENT] == IORregister[TIMERMAX])
		{
			IORregister[IRQ0STATUS] = 1;
			IORregister[TIMERCURRENT] = 0;
		}
		else IORregister[TIMERCURRENT] = IORregister[TIMERCURRENT] + 1;
	}
	
}

// input arg1: file irq2
// input arg2: the IOregisters array
// input arg3: clock
// input arg4: pointer to a counter named num
// output result: the function scanning the file irq2 in order to generate interrupt when needed. 
// the pointer to num is required in order to save the last spot which cause the interrupt.  
void routine_file(FILE *irq, int IORregister[], int clock, int* num)
{
	//int num=0;
	int endfile = 0; // 1 for end of file
	if (clock == 0) 
	{
		if (fscanf(irq, "%d", num) == -1)
		{
			endfile = 1;
		}
	}
	if (clock == *num && endfile==0)
	{
		IORregister[IRQ2STATUS] = 1;
		if (fscanf(irq, "%d", num) == -1)
		{
			endfile = 1;
		}
	}
	
}

// input arg1: IOregister array. 
// input arg2: pointer to an int- the timer disk 
// When an instruction is involving the disk, this function promote the timerdisk (counter) by 1 each clock cycle. 
// After the action complete (after 1024 clock cycle), the function generating an interrupt. 
void routine_disk(int IORegister[] ,int* timerdisk)
{
	if (IORegister[DISKSTATUS] == 1)
	{
		*timerdisk= *timerdisk +1;
		if (*timerdisk == 1023) //maybe 1024. have to check. 
		{
			IORegister[IRQ1STATUS] = 1;
			IORegister[DISKCMD] = 0;
			IORegister[DISKSTATUS] = 0;
			*timerdisk = 0;
		}
	}
}


//this function move the pointer in file to PC address
//input arg1: pointer to file
//input arg2: the required PC address the user want to move the pointer for
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

//the function doing the instruction that matches the opcodes 0-13
//input arg1: registers array 
//input arg2-7: variabels of the current instruction including- opcode, number of registers (rd, rs, rt), pc and the next pc (as a pointer)
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

//the function doing the instruction that matches the opcodes 14-15, in this case gets (lw) or writes (sw) information in memout
//input arg1: registers array 
//input arg2-7: variabels of the current instruction including- opcode, number of registers (rd, rs, rt), pc and the next pc (as a pointer)
//input and result arg8: poniter to memout file
void Instructions_lw_sw(int R[], int opcode, int rd, int rs, int rt, int PC, int* PC_next, FILE* Memout)
{
	switch (opcode) {
	case LW:
	{
		Jump_to_PC(Memout, R[rs] + R[rt]);
		fscanf(Memout, "%X\n", &R[rd]); 
		break;
	}
	case SW: 
	{
		Jump_to_PC(Memout, R[rs] + R[rt]);
		fprintf(Memout, "%08X\n", R[rd]);
		break;
	}
	}
}

//the function doing the instruction that matches the opcodes 16-18
//input arg 1-5: pointer to files 
//input arg 6: opcode 
//input arg7: array of registers 
//input arg8: array of IO registers
//input arg9-13: variabels of the current instruction including-  number of registers (rd, rs, rt),  next pc (as a pointer), clock cycle
void IO_Instructions(FILE* hwregtrace,FILE* leds, FILE* diskout,FILE* memout,FILE* display,int opcode, int R[], int IORegister[], int rd, int rs, int rt, int* PC_next, int clock_cycle) {
	int prev_leds = IORegister[LEDS];
	int prev_display = IORegister[DISPLAY];
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
		if (((R[rs] + R[rt]) < MAX_IOREG )&& (0<=(R[rs] + R[rt]))) {
			IORegister[R[rs] + R[rt]] = R[rd];
			fprintf(hwregtrace, "%d WRITE %s %08x\n", clock_cycle, (IOR_name + R[rs] + R[rt]), R[rd]);//printing to file writing to registers operation

			if (IORegister[LEDS] != prev_leds) //if leds register changed, write to leds file
				fprintf(leds, "%d %08X\n", clock_cycle, IORegister[LEDS]);

			if (IORegister[DISPLAY] != prev_display) //if display register changed, write to display file
				fprintf(display, "%d %08X\n", clock_cycle, IORegister[DISPLAY]);

			if ((IORegister[DISKSTATUS] == 0) && (IORegister[DISKCMD] == 1 || IORegister[DISKCMD] == 2)) {//checking if disk is free and if its read/write command to disk
				read_write_to_disk(diskout, memout, IORegister[DISKCMD], IORegister[DISKSECTOR], IORegister[DISKBUFFER]);
				IORegister[DISKSTATUS] = 1;//disk is busy
			}
		}
		break;
	}
}

//printing to trace file each clock cycle -pc address intruction, and all the data in the registers.
//input arg1: pointer to file for printing
//input arg2-7: variabels of the current instruction including-pc, opcode, rd, rs,rt,immediate
//input arg 8- the current registers array
void print_trace(FILE* trace,int PC,int opcode,int rd,int rs,int rt,int imm,int R[]) {

	fprintf(trace, "%08X ", PC);
	fprintf(trace, "%02X%01X%01X%01X%03X ", opcode, rd, rs, rt, MASK_REG(imm));

	for (int i = 0; i < MAX_REG; i++)//printing registers
		fprintf(trace, "%08X ", R[i]);
	fprintf(trace, "\n");
}
//this function prints to a file the data in registers R2-R15
//input arg1: pointer to file for printing
//input arg2 :array of the registers 
void print_regout(FILE *regout,int R[]) {
	int i = 2;//

	for (i = 2; i < MAX_REG; i++) 
		fprintf(regout, "%08X\n", R[i]);
}
//this function read/write to disk 
// input arg1: pointer to file disk
//input arg2: pointer to memory file
//input arg3: the command for function: 0 = no command, 1 = read sector, 2 = write sector
//input arg4:sector number of disk, range of sectors:0-127
//input arg5:Memory address of a buffer containing the sector being read or written.
void read_write_to_disk(FILE* diskout, FILE* memout, int diskcmd, int disksector, int diskbuffer) {
	int count = 0;
	char line[9] = { 0 };

	Jump_to_PC(diskout, disksector * 128);//jump to disksector in disk
	Jump_to_PC(memout, diskbuffer);//jump to diskbuffer in memory

	if (diskcmd == 1) {//read sector
		while (count < 128) {
			fscanf(diskout, "%s\n", line);
			fprintf(memout, "%s\n", line);
			count++;}
	}
	if (diskcmd == 2) //write sector
	{
		while (count < 128) {
			fscanf(memout, "%s\n", line);
			fprintf(diskout, "%s\n", line);
			count++;}
	}
}

//this function adding zero rows to file until there is (128*128) rows in file
//input: pointer to file
void adding_zeros_rows(FILE* f) {
	int count_line = 0;
	char line[9] = { 0 };
	int valid = 0;

	fseek(f, 0, SEEK_SET);
	while (!feof(f)) {
		valid= fscanf(f, "%s\n", line);
		if (valid != -1)//check if the line is not empty
		count_line++;
	}
	while (count_line < (128 * 128)) {
		fprintf(f, "00000000\n");
		count_line++;
	}
}
//this function duplicate source file to target file
//input arg1: pointer to file the user want to duplicate
//input arg2: pointer to the target file
void Copy_Text_File(FILE* source, FILE* target) {
	char line[9] = { 0 };
	int valid = 0;
	while (!feof(source)) {
		valid= fscanf(source, "%s\n", line);
		if(valid != -1)//check if the line is not empty
		fprintf(target, "%s\n", line);
	}
}