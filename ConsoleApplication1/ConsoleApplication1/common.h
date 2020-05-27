#ifndef COMMON_H
#define COMMON_H

#define SIZE 517
#define MAX_lEN_NAME 500

#define MAX_OPCODE 20
#define MAX_LEN_OPCODE 5

#define MAX_REG 16
#define MAX_LEN_REG 6




typedef struct Label label;
 struct Label {
	char name[MAX_lEN_NAME];
	int adress;
	label* next;
};


typedef struct HashTable {
	label** items;
	int size;
}HashTable;


HashTable* create_table();
void free_table(HashTable* table);
void insert_label(HashTable* table, char* label_name, int  adress);
void print_table(HashTable* table);
int get_adress_from_label(HashTable* table, char* label_name);
int get_opcode(char* str);
int get_reg(char* str);



#endif // !1

