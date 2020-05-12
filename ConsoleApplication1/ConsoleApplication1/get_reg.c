#include <stdio.h>
#define MAX_REG 3
#define MAX_LEN_REG 10

int get_reg(char* str) {

	char A[MAX_REG][MAX_LEN_REG] = { "ADD","b" ,"c"};
	int i = 0;

	for (i = 0; i < MAX_REG; i++) {
		printf("%s == ? %u\n", A[0],&A[0][0]);

	}
	return 0;
}