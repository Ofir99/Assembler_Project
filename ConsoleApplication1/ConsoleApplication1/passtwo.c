#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#define MAX_LINE 4096 //starts from zero to max_line-1
void zerostr(char str[]) //zero array
{
	int i = 0;
	int len = strlen(str);
	for (i; i < len; i++) {
		str[i] = 0;
	}
}

 PassTwo(int argc, char* argv[])
{
	char instruction[60] = { 0 };
	char firstString[6] = { 0 };
	char immBefore[51] = { 0 }; //last spot for \0
	int adress = 0;
	
	HashTable* table = NULL;
	table = passoneMain(argc, argv);
	
	//in args: test1.txt write.txt memin.txt
	FILE *f1 = fopen(argv[2], "r");
	FILE *memin = fopen(argv[3], "w");

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
			for (int i = 6; i < len-1; i++)
			{
				immBefore[i-6] = instruction[i];
			}

			//if label - returns code from table, else returns -1
			adress = get_adress_from_label(table, immBefore);

			if (adress == -1) //if number
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
	fclose(memin);
	free_table(table);
}




//#define _CRT_SECURE_NO_DEPRECATE
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include "common.h"
//
//void main(int argc, char* argv[])
//{
//	char firstString[5] = { 0 };
//	char immBefore[51] = { 0 }; //last spot for \0
//	char cl = '0';
//	int adress = 0;
//	int count = 0;
//	HashTable* table = NULL;
//	table = passoneMain(argc, argv);
//	//in args: test1.txt write.txt memin.txt
//	FILE *f1 = fopen(argv[2], "r");
//	FILE *memin = fopen(argv[3], "w");
//
//	if (f1 == NULL || memin == NULL)
//	{
//		printf("Cant open the file\n");
//		free_table(table);
//		exit(1);
//	}
//	
//	while (!feof(f1))
//	{
//		fscanf(f1, "%s", firstString); //string untill space opcode-rd-rs-rt
//		
//		//printf(firstString);
//		count = 0;
//		while ((cl = getc(f1)) != '\n') //&& !feof(f1)) //get imm - as a string (\0)
//		{
//			if (cl != ' ') //if it continues after the fscanf including the space
//			{
//				immBefore[count] = cl;
//				count++;
//			}
//		}
//		immBefore[count] = '\0';
//
//		fprintf(memin, "%s", firstString); //write to memin all the line but the imm
//
//		//check if label or number
//		//if label - returns code from table, else returns -1
//		adress=get_adress_from_label(table, immBefore);
//		
//		if (adress == -1) //if number
//		{
//			//if it was already a number it was in hexa
//			fprintf(memin, "%03s\n", immBefore); //only the first 3 chars? no \0
//		}
//		else //if label
//		{
//			//adress is int, write as hexa
//			fprintf(memin, "%03X\n", adress);
//		}
//	}
//
//	fclose(f1);
//	fclose(memin);
//
//	free_table(table);
//}