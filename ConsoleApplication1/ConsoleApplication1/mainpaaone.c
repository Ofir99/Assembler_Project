#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

int checking_label(char str[]) //  checking if its lable or not, returnning 1 if its a label, or 0 otherwise. 
{
	int i = 0;
	while (str[i] != '#' && str[i] != '\0')
	{
		if (str[i] == ':')
			return i+1;
		i = i + 1;

	}
	return 0;

}

void handling_imm(char imm[], char new[])
{
	int  j = 0;
	char pre[4];
	int len = strlen(imm);
	int k = 0;
	int i = 2;
	if ((imm[1] == 'x' || imm[1] == 'X') && imm[0] == 48) //the imm is a hex number
	{

		for (k; k < 5 - len; k++)
		{
			pre[k] = '0';
		}
		while (imm[i] != '\0')
		{
			pre[k] = imm[i];
			i = i + 1;
			k = k + 1;


		}
		pre[k] = '\0';
		strcpy(new, pre);
		return;


	}

	if ((imm[0] < 48 || imm[0]>57)&& imm[0]!= '-') // imm is a label
	{
		strcpy(new, imm);
		return;
	}

	// its a number

	int a = atoi(imm);
	
	sprintf(new, "%03X", a);
	if (imm[0] == '-')
	{
		int leni = strlen(new);
		char temp[4] = { '0' };
		temp[2] = new[leni-1];
		temp[1] = new[leni - 2];
		temp[0] = new[leni - 3];
		temp[3] = '\0';
		strcpy(new, temp);
	}


}

void clean_label(char ins[], char label[])
{
	int i = 0, j = 0;
	while (ins[i] != ':')
	{
		if (ins[i] != ' ' && ins[i] != '	')
		{
			label[j] = ins[i];
			j = j + 1;
		}
		i = i + 1;

	}
	label[j] = '\0';
}

//void decimal_to_binary(int n, char binary[], int k) // binary of k-1 bits
//{
//	int c, d, count;
//
//
//	count = 0;
//
//	for (c = k; c >= 0; c--)
//	{
//		d = n >> c;
//
//		if (d & 1)
//			binary[count] = 1 + '0';
//
//		else
//			binary[count] = 0 + '0';
//
//		count++;
//	}
//	binary[count] = '\0';
//
//
//}

//int extract_first(char ins[], char first[]) //extract the instruction word, returning the end index of it. 
//{
//	int i = 0, j = 0;
//	while (ins[i] == ' ' || ins[i] == '	')
//		i = i + 1;
//	while (ins[i] != ' ' && ins[i] != '	')
//	{
//		first[j] = ins[i];
//		j = j + 1;
//		i = i + 1;
//	}
//	first[j] = '\0';
//	return i;
//}

int extract_next(char ins[], char reg[], int counter)
{
	int j = 0;
	int counterf = counter;
	while (ins[counterf] == ' ' || ins[counterf] == ',' || ins[counterf] == '	')
	{
		counterf = counterf + 1;
	}
		
	if (ins[counterf] =='\0' || ins[counterf]=='#' || counterf==500|| ins[counterf]=='\n')
	{
		return -1;
	}
	while (ins[counterf] != ' ' && ins[counterf] != ',' && ins[counterf] != '#' && ins[counterf] != '	')
	{
		reg[j] = ins[counterf];
		j = j + 1;
		counterf = counterf + 1;
	}

	reg[j] = '\0';
	return counterf;
}



void restart_mem(char mem[4096][56])
{
	int i = 0;
	for (i; i < 4097; i++) {
		for (int j = 0; j < 8; j++)
		{
			mem[i][j] = '0';
		}
		mem[i][8] = '\0';
	}
}


void handling_word(char mem[4096][56], char reg1[], char reg2[])
{
	//reg1 and re2 is a number
	int m, d; 
	char new[9] = { 0 };
	if (reg1[1] == 'x' || reg1[1] == 'X')
	{
		m= (int)strtol(reg1, NULL, 0);
	}
	else
	{
		m = atoi(reg1);
	}
	if (reg2[1] == 'x' || reg2[1] == 'X')
	{
		d = (int)strtol(reg2, NULL, 0);
	}
	else
	{
		d = atoi(reg2);
	}
	sprintf(new, "%08X", d);
	strcpy(mem[m], new);

	
}
//void main HashTable* passoneMain( int argc, char* argv[])

void printingon_txt(FILE* f, char mem[4096][56])
{
	for (int i = 0; i < 4097; i++)
	{
		fprintf(f, "%s\n", mem[i]);
	}
	
}

HashTable* passoneMain( int argc, char* argv[])
{
	char instruction[500] = { 0 };
	char instruction_new[56] = { 0 };
	char label[50] = { 0 }, first[10] = { 0 }, reg1[10] = { 0 }, reg2[10] = { 0 }, reg3[10] = { 0 }, new_imm[10] = { 0 };
	char imm[500] = { 0 };
	int counter = 0;
	char mem[4096][56] = { 0 };
	restart_mem(mem);
	
	int opcode = 0, regg1 = 0, regg2 = 0, regg3 = 0;
	HashTable* table = NULL;
	table= create_table();
	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "w");
	int PC = 0;
	int label_c;
	
	if (f1 == NULL || f2 == NULL)
	{
		printf("Cant open the file\n");
		free_table(table);
		exit(1);
	}

	while (fgets(instruction, 500, f1) != NULL)
	{
		label_c = checking_label(instruction);
		if (label_c != 0) //  its a label 
		{
			clean_label(instruction, label);
			
			insert_label(table, label, PC);
			
			
		}
		
		
			counter = extract_next(instruction, first, label_c);
			
			if (counter != -1)
			{
				
				 
					counter = extract_next(instruction, reg1, counter);
					counter = extract_next(instruction, reg2, counter);
					if (first[0]=='.') // its .word
					{
						handling_word(mem, reg1, reg2);
					}
					else 
					{
						counter = extract_next(instruction, reg3, counter);
						extract_next(instruction, imm, counter);

						handling_imm(imm, new_imm);
						opcode = get_opcode(first);


						regg1 = get_reg(reg1);
						regg2 = get_reg(reg2);
						regg3 = get_reg(reg3);
						sprintf(instruction_new, "%02X%X%X%X %s", opcode, regg1, regg2, regg3, new_imm);
						
						strcpy(mem[PC], instruction_new);
						
						PC = PC + 1;
					}
				
			}
			
	}

	printingon_txt(f2, mem);

	fclose(f1);
	fclose(f2);
	
	//free_table(table);
	return table;
}

