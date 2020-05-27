#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_REG 16
#define INSTRUCTION_LEN 9

//this function simulates a SIMP processor 
//input argument: file that created in assembler function
void Simulator(FILE* Memin) {
	int R[MAX_REG] = { 0 };
	int opcode = 0;
	int	reg_rd = 0;
	int reg_rs = 0;
	int reg_rt = 0;
	int PC = 0;
	int PC_next = 0;
	char instruction[INSTRUCTION_LEN] = {0};



	//instruction= getting line from file



}