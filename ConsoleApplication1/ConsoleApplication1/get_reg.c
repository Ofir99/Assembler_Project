#include <stdio.h>
#include <string.h>
#define MAX_REG 16
#define MAX_LEN_REG 6

int get_reg(char* str) {

	char A[MAX_REG][MAX_LEN_REG] = { "$zero","$imm" ,"$v0","$a0" ,"$a1" ,"$t0" ,"$t1" ,"$t2" ,"$t3" ,"$s0" ,"$s1" ,"$s2" ,"$gp" ,"$sp" ,"$fp" ,"$ra" };
	int i = 0;

	for (i = 0; i < MAX_REG; i++) {
		if (strcmp(str, A + i) == 0) {
			return i;
		}
	}
	printf("error:no register was found\n");
	return -1;
}