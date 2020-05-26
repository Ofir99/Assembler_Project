#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#define firstStr 5

void passTwoMain(int argc, char* argv[])
{
	char firstString[5] = { 0 };
	char immBefore[4] = { 0 }; //last spot for \0
	char immAfter[3] = { 0 };
	int adress = 0;
	HashTable* table = NULL;
	table = passoneMain();
	//in args: test.txt write.txt memin.txt
	FILE *f1 = fopen(argv[2], "r");
	FILE *memin = fopen(argv[3], "w");
	immBefore[4] = '\0';

	if (f1 == NULL || memin == NULL)
	{
		printf("Cant open the file\n");
		free_table(table);
		exit(1);
	}
	while (!feof(f1))
	{
		fscanf(f1, firstString);
		fscanf(f1, "%s",immBefore); //take original imm

		fprintf(memin, "%s", firstString); //write to memin all the line but the imm

		//check if label or number
		//if label(\0) find code in table and put in immAfter
		adress=get_adress_from_label(table, immBefore);
		
		//if number (adress=-1) immAfter=immBefore
		if (adress == -1)
		{
			fprintf(memin, "%03s\n", immBefore); //only the first 3 chars? no \0
		}
		else
		{
			//adress from int to string -> inmmAfter
			fprintf(memin, "%03X\n", adress);
		}

	}
}