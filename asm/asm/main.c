#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

//input1: amount of input files
//input 2: argv[]={program.exe,test.asm, memin.txt}
//output: creating "memin.txt", the file represent the main memory following test.asm, each line's number is the pc adress of the memory and the data in it.
int main(int argc,char* argv[]) {
	HashTable* table = NULL;
	FILE *test_file = fopen(argv[1], "r");
	FILE *memin = fopen(argv[2], "w");

	table = create_table();
	PassOne(test_file, table);
	PassTwo(memin,table);

	free_table(table);
	fclose(test_file);
	fclose(memin);
}


//input 1: f1 is the test.asm file
//input 2:pointer to an empty Hashtable
//output: creating a temporary file for PassTwo
void PassOne(FILE* f1, HashTable* table)
{
	char instruction[500] = { 0 };
	char instruction_new[56] = { 0 };
	char label[50] = { 0 }, first[10] = { 0 }, reg1[12] = { 0 }, reg2[12] = { 0 }, reg3[10] = { 0 }, new_imm[50] = { 0 };
	char imm[500] = { 0 };
	int counter = 0;
	char mem[4096][56] = { 0 };
	restart_mem(mem);

	int opcode = 0, regg1 = 0, regg2 = 0, regg3 = 0;

	FILE *f2 = fopen("write.txt", "w");//creating a temporary file for PassTwo
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
			insert_label(table, label, PC);}
			counter = extract_next(instruction, first, label_c);
		if (counter != -1)
		{
			counter = extract_next(instruction, reg1, counter);
			counter = extract_next(instruction, reg2, counter);
			if (first[0] == '.') // its .word
				handling_word(mem, reg1, reg2);
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
				PC = PC + 1;}}
	}
	printingon_txt(f2, mem);
	fclose(f2);
	return;
}

//this function gets an array of chars and intialize the array to 0
//input 1: array of char characters
void zerostr(char str[]) 
{
	int i = 0;
	int len = strlen(str);
	for (i; i < len; i++) {
		str[i] = 0;
	}
}

//this function reading from the temporary file that created in PassOne and writing to a new file(memin) the same content, but only changing the label's name with their PC address
//input 1: memin is an empty text file for writing 
//input 2: pointer to table with labels that filled in PassOne
void PassTwo(FILE* memin, HashTable* table)
{
	char instruction[60] = { 0 };
	char firstString[6] = { 0 };
	char immBefore[51] = { 0 }; //last spot for \0
	int adress = 0;

	//in args: test1.txt write.txt memin.txt
	FILE* f1 = fopen("write.txt", "r");

	if (f1 == NULL || memin == NULL)
	{
		printf("Cant open the file\n");
		free_table(table);
		exit(1);
	}
	while (fgets(instruction, 60, f1) != NULL)
	{
		int len = strlen(instruction);

		if (instruction[5] != ' ')
		{
			fprintf(memin, "%s", instruction);
		}
		else
		{
			zerostr(firstString);
			zerostr(immBefore);
			for (int i = 0; i < 5; i++)
			{
				firstString[i] = instruction[i];
			}
			for (int i = 6; i < len - 1; i++)
			{
				immBefore[i - 6] = instruction[i];
			}

			//if label exists - returns its adress from the table, else returns -1
			adress = get_adress_from_label(table, immBefore);

			if (adress == -1) //if not a label, probably a number
			{
				//if it was already a number it was in hexa
				fprintf(memin, "%s%03s\n", firstString, immBefore);
			}
			else //if label
			{
				//adress is int, write as hexa
				fprintf(memin, "%s%03X\n", firstString, adress);
			}
		}
	}

	fclose(f1);
}


// input args: input arg1- pointer to file f- file f is the temporary file which will be transfer to pass2. 
// input arg2- mem[][]- array of arrays, the array constains all the memory which will be transfer forward. 
// result: the function is printing the array into the temporary file f. 
void printingon_txt(FILE* f, char mem[4096][56])
{
	for (int i = 0; i < 4096; i++)
		fprintf(f, "%s\n", mem[i]);
}


// input args: input arg1- mem[][]- array of arrays of the memory. 
// input arg2- reg1[]- memory address.
// input arg3- reg2[]- data.
// result: This function handling .word instruction. after this function the right cell with reg1 address will contain the data reg2. 
void handling_word(char mem[4096][56], char reg1[], char reg2[])
{
	//reg1 and re2 is a number
	int m, d, i=2, j=0;
	char per[9] = { 0 };
	char new[9] = { 0 };
	
	if (reg1[1] == 'x' || reg1[1] == 'X')
	{
		m = (int)strtol(reg1, NULL, 0);
	}
	else
	{
		m = atoi(reg1);
	}
	if ((reg2[1] == 'x' || reg2[1] == 'X') && strlen(reg2)==10)
	{

		
		while (reg2[i]!='\0')
		{
			per[j] = reg2[i];
			i++;
			j++;

		}
		per[j] = '\0';
		strcpy(new, per);

	}
	else if ((reg2[1] == 'x' || reg2[1] == 'X') && strlen(reg2) != 10)
	{
		d = (int)strtol(reg2, NULL, 0);
		sprintf(new, "%08X", d);
	}
	else
	{
		d = atoi(reg2);
		sprintf(new, "%08X", d);
		
	}
	if (m > 4096)
		m = 4096;

	strcpy(mem[m], new);
}


//input arg- an array of arrays. After pass two function, the array will include all the memory which will be transfer forward.  
// result arg- this function restart the array of array to be arrays of zeroes. 
void restart_mem(char mem[4096][56])
{
	int i = 0;
	for (i; i < 4096; i++) {
		for (int j = 0; j < 8; j++)
		{
			mem[i][j] = '0';
		}
		mem[i][8] = '\0';
	}
}

//this function prpuse is to clean and extract the entire relevant parts of the instruction. 
// input arg: 
// input arg1 - ins[]- the current instruction 
// input arg2- reg[]- a new arrey, which will be contain the relevant part which now extract. 
// input arg3- counter- the place in the instruction which we would like to extract the next part. (example: add $sp, $zero, $imm, 1  ----> in order to extract $sp, the counter will be in 3 )
// result arg- reg[] array will conatain the relevant part of the instruction, after cleaning none relevant (',' , '#'.. etc.)
int extract_next(char ins[], char reg[], int counter)
{
	int j = 0;
	int counterf = counter;
	while (ins[counterf] == ' ' || ins[counterf] == ',' || ins[counterf] == '	')
	{
		counterf = counterf + 1;
	}

	if (ins[counterf] == '\0' || ins[counterf] == '#' || counterf == 500 || ins[counterf] == '\n')
	{
		return -1;
	}
	while (ins[counterf] != ' ' && ins[counterf] != ',' && ins[counterf] != '#' && ins[counterf] != '	' && ins[counterf] != '\n' && ins[counterf] != '\0')
	{
		reg[j] = ins[counterf];
		j = j + 1;
		counterf = counterf + 1;
	}

	reg[j] = '\0';
	return counterf;
}

// input arg- str[]- array of the imm 
//  checking if its lable or not, returnning 1 if its a label, or 0 otherwise. 
int checking_label(char str[]) 
{
	int i = 0;
	while (str[i] != '#' && str[i] != '\0')
	{
		if (str[i] == ':')
			return i + 1;
		i = i + 1;

	}
	return 0;

}

// input arg:
// input arg1- imm[]- the imm register as extracted from the assembly code. 
// input arg2- new[] - new array. 
// result arg- the function will handle the imm matter: hex or int negative or possitive will be convert here to hex 3 bits long. If its a label, will remain the same. 
void handling_imm(char imm[], char new[])
{
	int  j = 0;
	char pre[4] = "000";
	//pre[3] = '\0';
	int len = strlen(imm);
	int k = len-1;
	int i = 2; //2
	if ((imm[1] == 'x' || imm[1] == 'X') && imm[0] == 48) //the imm is a hex number 
	{
		
		while (imm[k] != 'x' && imm[k] != 'X'&& i>-1)
		{
			pre[i] = imm[k];
			i--;
			k--;
		}
		strcpy(new, pre);
		return;

	}

	if ((imm[0] < 48 || imm[0]>57) && imm[0] != '-') // imm is a label
	{
		strcpy(new, imm);
		return;
	}

	// its a number

	int a = atoi(imm);

	sprintf(new, "%03X", a);
	if (imm[0] == '-' || a>4095)
	{
		int leni = strlen(new);
		char temp[4] = { '0' };
		temp[2] = new[leni - 1];
		temp[1] = new[leni - 2];
		temp[0] = new[leni - 3];
		temp[3] = '\0';
		strcpy(new, temp);
	}


}

// input args: input arg1 - ins[]- the line which currently handling by passone. In case that this function was called, the line contains label. So ins[]= lable 
// input arg2- lable[]- a new array. 
// result arg: the function is cleanning the lable, and restore the clean lable in the array label[]. 
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




//input: string contains the name of the register example: "$s1"
//output: int that represent the Register Number
int get_reg(char* str) {

	char A[REG_SIZE][STR_REG_LEN] = { "$zero","$imm" ,"$v0","$a0" ,"$a1" ,"$t0" ,"$t1" ,"$t2" ,"$t3" ,"$s0" ,"$s1" ,"$s2" ,"$gp" ,"$sp" ,"$fp" ,"$ra" };
	int i = 0;

	for (i = 0; i < REG_SIZE; i++) {
		if (strcmp(str, A + i) == 0) {
			return i;
		}
	}
	printf("error:no register was found\n");
	return -1;
}

//input: string contains the name of the opcode example: "add" **the name should be only in lower case
//output: int that represent the Opcode Number
int get_opcode(char* str) {

	char A[OPCODE_SIZE][STR_OPCODE_LEN] = { "add","sub" ,"and","or" ,"sll" ,"sra" ,"srl" ,"beq" ,"bne" ,"blt" ,"bgt" ,"ble" ,"bge" ,"jal" ,"lw" ,"sw" ,"reti" ,"in" ,"out" ,"halt" };
	int i = 0;

	for (i = 0; i < OPCODE_SIZE; i++) {
		if (strcmp(str, A + i) == 0) {
			return i;
		}
	}
	printf("error:no opcode was found\n");
	return -1;
}


//creating new label using a dynamic allocation 
//input arg1: label's name
//input arg2: the PC adress the label was found
//output: pointer to the new label from type 'label'
static label* create_label(char* name, int adress) {
	label* new_label = NULL;
	new_label = (label*)malloc(sizeof(label));
	if (new_label != NULL) {

		new_label->adress = adress;
		strcpy_s(new_label->name, MAX_lEN_NAME, name);
		new_label->next = NULL;
	}
	else {
		printf("allocation problen for label");
	}
	return new_label;
}

// Creates a new HashTable and reseting the table to NULL
//output: pointer to a new and empty Hashtable
HashTable* create_table() {// size should be prime number

	int i = 0;

	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	NULL_ERROR(table)

		table->size = HASHTABLE_SIZE;
	table->items = (label**)malloc(HASHTABLE_SIZE * sizeof(label*));

	for (i = 0; i < HASHTABLE_SIZE; i++) {
		*(table->items + i) = NULL;

	}

	return table;
}
//this function free's label and it's linked list
//input: item from type 'label'
static void free_item(label* item) {
	if (item == NULL) {
		return;
	}
	free_item(item->next);
	free(item);

}
// Free's the table and all the pointers the table contains 
//input: pointer to Hashtable
void free_table(HashTable* table) {

	for (int i = 0; i < table->size; i++) {
		label* item = table->items[i];
		if (item != NULL) {
			free_item(item);
		}
	}

	free(table->items);
	free(table);
}



/* this function gives a unique hash code(int) to the given key(label's name) */
//input: string
//output: integer that represent the unique hash code of the string
static int  hashcode(char* label_name)
{
	int i = 0;
	unsigned int hash = 0;
	for (i = 0; *(label_name + i) != '\0'; i++)
	{
		hash = hash * 7 + label_name[i];
	}
	return hash % HASHTABLE_SIZE;
}

//this function adding a new label to an exiting table by getting 3 argumets=(table, name's label , PC adress)
//the function creating a new lable from type label , finding a uniqe hashcode to the label, and inserting the label to the hashtable.
void insert_label(HashTable* table, char* label_name, int  adress)
{
	label* exist_label = NULL;
	label* new_label = NULL;
	int hash = 0;
	hash = hashcode(label_name);
	new_label = create_label(label_name, adress);

	if (table->items[hash] == NULL) {//if the cell is empty
		table->items[hash] = new_label;
		return;
	}
	exist_label = table->items[hash];//case of collision

	while (exist_label->next != NULL) {
		exist_label = exist_label->next;
	}
	exist_label->next = new_label;
	return;



}

//this function is for debuging
//the function prints all the labels in the given table 
void print_table(HashTable* table) {
	label* temp = NULL;
	int i = 0;
	for (i = 0; i < HASHTABLE_SIZE; i++) {

		temp = *(table->items + i);
		while (temp != NULL) {

			printf("label's name is: %s, adress: %d, hash: %d \n", temp->name, temp->adress, i);
			temp = temp->next;
		}
	}
}

//this function is for PassTwo
//the function return the PC adress from an exiting table of a given label's name, if the label not in the table it returns (-1)
//input 1: pointer to Hashtable
//input 2: string that contains the label's name for searching in the table
//output: if the label was found it returns it's pc address, else -1
int get_adress_from_label(HashTable* table, char* label_name) {
	label* temp = NULL;
	int hash = 0;
	hash = hashcode(label_name);
	temp = table->items[hash];
	while (temp != NULL) {
		if (strcmp(temp->name, label_name) == 0) {
			return table->items[hash]->adress;
		}
		temp = temp->next;
	}
	//printf("ERROR: LABEL [%s] NOT FOUND\n", label_name);
	return -1;
}
