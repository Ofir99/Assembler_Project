#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h";

int checking_label(char str[]) //  checking if its lable or not, returnning 1 if its a label, or 0 otherwise. 
{
	int i = 0;
	while (str[i] != '#' && str[i] != '\0')
	{
		if (str[i] == ':')
			return 1;
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

	if (imm[0] < 48 || imm[0]>57) // imm is a label
	{
		strcpy(new, imm);
		return;
	}

	// its a number

	int a = atoi(imm);
	sprintf(new, "%03X", a);



}

void clean_label(char ins[], char label[])
{
	int i = 0, j = 0;
	while (ins[i] != ':')
	{
		if (ins[i] != ' ')
		{
			label[j] = ins[i];
			j = j + 1;
		}
		i = i + 1;

	}
	label[j] = '\0';
}

void decimal_to_binary(int n, char binary[], int k) // binary of k-1 bits
{
	int c, d, count;


	count = 0;

	for (c = k; c >= 0; c--)
	{
		d = n >> c;

		if (d & 1)
			binary[count] = 1 + '0';

		else
			binary[count] = 0 + '0';

		count++;
	}
	binary[count] = '\0';


}

int extract_first(char ins[], char first[]) //extract the instruction word, returning the end index of it. 
{
	int i = 0, j = 0;
	while (ins[i] == ' ' || ins[i] == '	')
		i = i + 1;
	while (ins[i] != ' ' && ins[i] != '	')
	{
		first[j] = ins[i];
		j = j + 1;
		i = i + 1;
	}
	first[j] = '\0';
	return i;
}

int extract_next(char ins[], char reg[], int counter)
{
	int j = 0;
	int counterf = counter;
	while (ins[counterf] == ' ' || ins[counterf] == ',' || ins[counterf] == '	')
		counterf = counterf + 1;
	while (ins[counterf] != ' ' && ins[counterf] != ',' && ins[counterf] != '#' && ins[counterf] != '	')
	{
		reg[j] = ins[counterf];
		j = j + 1;
		counterf = counterf + 1;
	}

	reg[j] = '\0';
	return counterf;
}


int main()
{
	char instruction[500] = { 0 };
	char label[50] = { 0 };
	char first[10] = { 0 };
	char reg1[10] = { 0 };
	char reg2[10] = { 0 };
	char reg3[10] = { 0 };
	char imm[500] = { 0 };
	char new_imm[10] = { 0 };
	int counter = 0;
	int opcode = 0, regg1 = 0, regg2 = 0, regg3 = 0;
	HashTable* table = NULL;
	table= create_table();
	FILE *f1 = fopen("C:\\Users\\Mor\\source\\repos\\danielashabat\\Assembler_Project\\test1.txt", "r");
	FILE *f2 = fopen("C:\\Users\\Mor\\source\\repos\\danielashabat\\Assembler_Project\\write.txt", "w");
	int PC = 1;
	int label_c;
	
	if (f1 == NULL || f2 == NULL)
	{
		printf("Cant open the file\n");
	}
	while (fgets(instruction, 500, f1) != NULL)
	{
		label_c = checking_label(instruction);
		if (label_c == 1) //  its a label 
		{
			clean_label(instruction, label);
			//printf("%s\n", label);
			insert_label(table, label, PC);
			
			//sending to daniela's functions (label, PC+1)
		}
		else
		{
			counter = extract_first(instruction, first);
			counter = extract_next(instruction, reg1, counter);
			counter = extract_next(instruction, reg2, counter);
			counter = extract_next(instruction, reg3, counter);
			extract_next(instruction, imm, counter);

			handling_imm(imm, new_imm);
			opcode = get_opcode(first);
			//printf("%d", 3);
		
			regg1 = get_reg(reg1);
			regg2 = get_reg(reg2);
			regg3 = get_reg(reg3);
			fprintf(f2, "%02X", opcode);
			fprintf(f2, "%X", regg1);
			fprintf(f2, "%X", regg2);
			fprintf(f2, "%X", regg3);
			fprintf(f2, " %s\n", new_imm);
			PC = PC + 1;




			//int a = atoi(test);
			//printf("%d\n", a);
			//char hex[10] = { '0' };
			//sprintf(hex, "%03X", a);
			//printf("%s\n", hex);
			//PC = PC + 1;
			//printf("%x\n", hex);

			//int n=12 ;
			//char binary[33] = { '0' };


			//decimal_to_binary(n, binary);

			//printf("Binary string of %d is: %s\n", n, binary);



		}


	}


	//printf("%s\n", instruction);

	fclose(f1);
	fclose(f2);
	print_table();
	free_table();
	return 0;
}

