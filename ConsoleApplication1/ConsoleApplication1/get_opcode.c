#include <stdio.h>
#include <string.h>
#define MAX_OPCODE 20
#define MAX_LEN_OPCODE 5

int get_opcode(char* str) {

	char A[MAX_OPCODE][MAX_LEN_OPCODE] = { "add","sub" ,"and","or" ,"sll" ,"sra" ,"srl" ,"beq" ,"bne" ,"blt" ,"bgt" ,"ble" ,"bge" ,"jal" ,"lw" ,"sw" ,"reti" ,"in" ,"out" ,"halt" };
	int i = 0;

	for (i = 0; i < MAX_OPCODE; i++) {
		if (strcmp(str, A + i) == 0) {
			return i;
		}
	}
	printf("error:no opcode was found\n");
	return -1;
}