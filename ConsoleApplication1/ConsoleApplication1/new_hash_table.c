#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define NULL_ERROR(NAME) if (NAME == NULL) {\
	printf("table allocation failed\n");\
	exit(1);\
}


//input: string contains the name of the register example: "$s1"
//output: int that represent the Register Number
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

//input: string contains the name of the opcode example: "add" **the name should be only in lower case
//output: int that represent the Opcode Number
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


//creating new label using a dynamic allocation 
//input arg1:  will be label's name
//input arg2: the PC adress the label was found
//output: pointer to the new label
static label* create_label(char* name,int adress) {
	label* new_label = NULL;
	new_label = (label*)malloc(sizeof(label));
	if (new_label != NULL) {

		new_label->adress=adress;
		strcpy_s(new_label->name, MAX_lEN_NAME, name);
		new_label->next = NULL;
	}
	else {
		printf("allocation problen for label");
	}
	return new_label;
}

// Creates a new HashTable and reseting the table to NULL
HashTable* create_table() {// size need to be prime number
	
	int i = 0;
	
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	NULL_ERROR(table)

	table->size = SIZE;
	table->items = (label**)malloc(SIZE*sizeof(label*));

	for (i = 0; i < SIZE; i++) {
		*(table->items + i)=NULL ;
		
	}

	return table;
}
//this function free's label and it's linked list
static void free_item(label* item) {
	if (item == NULL) {
		return;
	}
	free_item(item->next);
	free(item);

}
// Free's the table and all the pointers the table contains 
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


//this function can't used outside this file
/* this function gives a unique hash code(int) to the given key(label's name) */
static int  hashcode(char* label_name)
{
	int i = 0;
	int hash = 0;
	for (i = 0; *(label_name+i) != '\0'; i++)
	{
		hash = hash*7 + label_name[i];
	}



	return hash%SIZE ;
}

//this function adding a new label to an exiting table by getting 3 argumets=(table, name's label , PC adress)
//the function creating a new lable from type label , finding a uniqe hashcode to the label, and inserting the label to the hashtable.
void insert_label(HashTable* table ,char* label_name,int  adress)
{
	label** adress__for_label = NULL;
	label* new_label = NULL;
	int hash = 0;
	hash = hashcode(label_name);
	new_label= create_label(label_name, adress);

	adress__for_label = (table->items + hash);
	if (*(adress__for_label) == NULL) {
		*(adress__for_label) = new_label;
		return;
	}
	
	while ((*adress__for_label)->next != NULL) {
		adress__for_label = (*adress__for_label)->next;
	}
	(*adress__for_label)->next= new_label;
	return;



}

//this function is for debuging
//the function prints all the labels in the given table 
void print_table(HashTable* table) {
	label* temp = NULL;
	int i = 0;
	for (i = 0; i < SIZE; i++) {

		temp = *(table->items+i);
		while (temp != NULL) {
				
				printf("label's name is: %s, adress: %d, hash: %d \n",temp->name, temp->adress, i);
				temp = temp->next;
			}
	}
}

//this function is for passtwo
//the function return the PC adress from an exiting table of a given label's name, if the label not in the table it returns (-1)
int get_adress_from_label(HashTable* table, char* label_name) {
	label* temp = NULL;
	int hash = 0;
	hash= hashcode(label_name);
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
