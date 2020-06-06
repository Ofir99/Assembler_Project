#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simulator.h"

//in simulator:
//if IORegister[14] != 0{
//	read_write_to_disk();
//}

void read_write_to_disk(FILE* diskout,FILE* memout,int diskcmd,int disksector,int diskbuffer) {
	//timer on to 1024
	int count = 0;
	char line[9] = { 0 };

	//IORegister[17] = 1;//changing diskstaus to busy
	Jump_to_PC(diskout, disksector * 128);//jump to disksector in disk
	Jump_to_PC(memout, diskbuffer);//jump to diskbuffer in memory

		if (diskcmd == 1) {//read sector
			while (count < 128) {
				fscanf(diskout, "%s\n", line);
				fprintf(memout, "%s\n", line);
				count++;
			}
		}
		else//write sector
		{
			while (count < 128) {
				fscanf(memout, "%s\n", line);
				fprintf(diskout, "%s\n", line);
				count++;
			}
		}

}

void adding_zeros_rows(FILE* f) {
	int count_line = 0;
	char line[9] = { 0 };
	while (!feof) {
		fscanf(f, "%s\n", line);
		count_line++;
	}
	while (count_line < (128 * 128)) {
		fprintf(f, "00000000\n");
		count_line++;
	}
}
