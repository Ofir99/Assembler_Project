#include <stdio.h>
#include "common.h"


int main() {
	label* new= NULL;
	int a = 0;
	HashTable* table = NULL;
	char* name = "or";
	
	table = create_table();
	
	insert_label(table, name, 50);
	insert_label(table, name, 90);
	insert_label(table, "daniela", 100);

	printf("hash is : %d  %s\n", hashcode(name), table->items[hashcode(name)]->name);
	printf("the adress is: %d\n",get_adress_from_label(table, name));

	print_table(table);
	free_table(table);
	scanf_s("%d", &a);

	return 0;
}