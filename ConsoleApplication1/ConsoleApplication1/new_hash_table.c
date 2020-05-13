#include <stdio.h>
#include <string.h>
#include "common.h"



label* create_label(char* name,int adress) {
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


HashTable* create_table() {// size need to be prime number
	// Creates a new HashTable
	int i = 0;
	
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	table->size = SIZE;
	table->items = (label**)malloc(SIZE*sizeof(label*));

	for (i = 0; i < SIZE; i++) {
		*(table->items + i)=NULL ;
		
	}

	return table;
}
//this function can't used outside this file
static void free_item(label* item) {
	if (item == NULL) {
		return;
	}
	free_item(item->next);
	free(item);

}

void free_table(HashTable* table) {
	// Frees the table
	for (int i = 0; i < table->size; i++) {
		label* item = table->items[i];
		if (item != NULL)
			free_item(item);
	}

	free(table->items);
	free(table);
}


//this function can't used outside this file
/* this function gives a unique hash code to the given key */
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

/* to insert a key in the hash table */
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


void print_table(HashTable* table) {
	label* temp = NULL;
	int i = 0;
	for (i = 0; i < SIZE; i++) {
		temp = *(table->items+i);
		while (temp != NULL) {
				
				printf("label's name is: %s, adress: %d, hash: %d \n ",temp->name, temp->adress, i);
				temp = temp->next;
			}
	}
}


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
	printf("ERROR: LABEL NOT FOUND\n");
	return -1;
}

