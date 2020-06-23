#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define HASHTABLE_SIZE 517
#define MAX_lEN_NAME 500

#define OPCODE_SIZE 20
#define STR_OPCODE_LEN 5

#define REG_SIZE 16
#define STR_REG_LEN 6

#define MAX_LINE 4096 //starts from zero to max_line-1

#define NULL_ERROR(NAME) if (NAME == NULL) {\
	printf("table allocation failed\n");\
	exit(1);\
}


// this struct represent label
// the labels are given from the tests programs

typedef struct Label label;
struct Label {
	char name[MAX_lEN_NAME];//field name - contains label's name
	int adress;//field address - contains the PC adress the label was found
	label* next;// this field is for dealing with collision in the hashtable
};

//this struct implement a Hashtable 
//in the Hashtale we save all the labels in "items"
typedef struct HashTable {
	label** items;// items is an array of pointers to labels 
	int size;// array's size is 'size' as the user decide
}HashTable;


HashTable* create_table();
void free_table(HashTable* table);
void insert_label(HashTable* table, char* label_name, int  adress);
int get_adress_from_label(HashTable* table, char* label_name);
int get_opcode(char* str);
int get_reg(char* str);
void PassOne(FILE* f1, HashTable* table);
void printingon_txt(FILE* f, char mem[4096][56]);
void handling_word(char mem[4096][56], char reg1[], char reg2[]);
void restart_mem(char mem[4096][56]);
int extract_next(char ins[], char reg[], int counter);
int checking_label(char str[]);
void handling_imm(char imm[], char new[]);
void clean_label(char ins[], char label[]);
void PassTwo(FILE* memin, HashTable* table);
void zerostr(char str[]);


#endif // !1

#pragma once
