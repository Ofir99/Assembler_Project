#ifndef COMMON_H
#define COMMON_H

#define SIZE 517
#define MAX_lEN_NAME 500


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






#endif // !1

