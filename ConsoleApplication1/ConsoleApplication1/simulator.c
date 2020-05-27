#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_REG 16
#define INSTRUCTION_LEN 10

void Extract_Variabales_from_PC(FILE* f, int  num_line, int* opcode, int* reg_rd, int* reg_rs, int* reg_rt, int* imm);
void Simulator(FILE* Memin);
void Branch_Jump_opcode(int R[], int opcode, int rd, int rs, int rt, int imm, int PC, int* PC_next);



int main(int argc, char* argv[]) {

	FILE* Memin = NULL;
	char line[INSTRUCTION_LEN];

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

	Extract_Variabales_from_PC(Memin, PC, &opcode, &rd, &rs, &rt, &imm);
	
	opcode = 13;

	if (opcode >= 7 && opcode <= 13) {
		Branch_Jump_opcode(R, opcode, rd, rs, rt, imm,PC, &PC_next);
	}

	//printf("%d", PC_next);


}



//the function allocates data to variabales from the PC adress
//input arg1: pointer to file , input arg2: number of line to read from(PC), input arg3,4,5,6,7:poniter to variabels
void Extract_Variabales_from_PC(FILE* f,int  PC,int* opcode,int* reg_rd,int* reg_rs,int* reg_rt, int* imm) {
	char instruction[INSTRUCTION_LEN] = { 0 };
	int count_line = 0;
	while (!feof(f)) {
		if (count_line == PC) {
			break;
		}
		fscanf(f, "%s\n", instruction);
		count_line++;
	}
	fscanf(f, "%02X%01X%01X%01X%03X\n", opcode, reg_rd, reg_rs, reg_rt, imm);//extracting values 
	return;
}

void Branch_Jump_opcode(int R[],int opcode,int rd,int rs,int rt,int imm,int PC, int* PC_next) {
	switch (opcode) {
	
	case 7:
		if (R[rs] == R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 8:
		if (R[rs] != R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 9:
		if (R[rs] < R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 10:
		if (R[rs] > R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 11:
		if (R[rs] <= R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 12:
		if (R[rs] >= R[rt]) {
			*PC_next = R[rd];//low bits
		}
		break;

	case 13:
		R[15] = PC + 1;
		*PC_next = R[rd];//low bits
		break;
	}
	



}