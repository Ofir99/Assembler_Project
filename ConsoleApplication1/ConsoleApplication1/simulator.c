#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simulator.h"


int main(int argc, char* argv[]) {

	FILE* Memin = NULL;
	Memin = fopen(argv[3], "r");
	Simulator(Memin);
	fclose(Memin);
	return 0;
}


//this function simulates a SIMP processor 
//input argument: file that created in assembler function
void Simulator(FILE* Memin) {
	int R[MAX_REG] = { 0 };
	int opcode = 0;
	int	rd = 0;
	int rs = 0;
	int rt = 0;
	int imm = 0;
	int PC = 0;
	int PC_next = 0;

	
	//while(true) loop until halt opcode  
	PC = PC_next;
	Extract_Variabales_from_PC(Memin, PC, &opcode, &rd, &rs, &rt, &imm);
	//printf("%02X %01X %01X %01X %03X\n", opcode, rd, rs, rt, imm);
	
	if (opcode >= 0 && opcode <= 6) {
		Instructions0to6_opcode(R, opcode, rd, rs, rt);
		/*printf("%01d %01d %01d\n",M[rd],M[rs],M[rt]);*/
	}
	if (opcode >= 7 && opcode <= 13) {
		Branch_Jump_opcode(R, opcode, rd, rs, rt, imm,PC, &PC_next);
	}
}


//this function move the pointer in file to PC address
void Jump_to_PC(FILE* f, int PC) {
	fseek(f, PC*10,SEEK_SET);//10 bytes in line 
}


//the function allocates data to variabales from the PC adress
//input arg1: pointer to file , input arg2: number of line to read from(PC), input arg3,4,5,6,7:poniter to variabels
void Extract_Variabales_from_PC(FILE* f,int  PC,int* opcode,int* rd,int* rs,int* rt, int* imm) {
	Jump_to_PC(f, PC);
	fscanf(f, "%02X%01X%01X%01X%03X\n", opcode, rd, rs, rt, imm);//extracting values 
	return;
}

void Branch_Jump_opcode(int R[], int opcode, int rd, int rs, int rt, int imm, int PC, int* PC_next) {
	switch (opcode) {

	case BEQ:
		if (R[rs] == R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case BNE:
		if (R[rs] != R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case BLT:
		if (R[rs] < R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case BGT:
		if (R[rs] > R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case BLE:
		if (R[rs] <= R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case BGE:
		if (R[rs] >= R[rt]) {
			*PC_next = MASK_REG(R[rd]);
		}
		break;

	case JAL:
		R[15] = PC + 1;
		*PC_next = MASK_REG(R[rd]);
		break;
	}
}
	void Instructions0to6_opcode(int R[], int opcode, int rd, int rs, int rt)//, int imm, int PC, int* PC_next)
	{
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
		{
			R[rd] = unsigned(R[rs]) >> R[rt];
			break;
		}
		}
	}
