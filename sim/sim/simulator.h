

#ifndef	SIMULATOR_H
#define SIMULATOR_H

#define MAX_REG 16
#define MAX_IOREG 18
#define INSTRUCTION_LEN 10
#define MASK_REG(R) R&0xFFF//save only the 12 low bits 
#define SIGNED_EXT_IMM(IMM) ((IMM>>11)&1) ? (int) (IMM|0xFFFFF000) : IMM;

#define $IMM 1

//IORegisters

#define IRQRETURN 7
#define CLKS 8
#define LEDS 9
#define DISPLAY 10
#define DISKCMD 14
#define DISKSECTOR 15
#define DISKBUFFER 16
#define DISKSTATUS 17

#define	ADD 0
#define	SUB 1
#define	AND 2
#define	OR 3
#define	SLL 4
#define	SRA 5
#define	SRL 6
#define	BEQ 7
#define BNE 8
#define BLT 9
#define BGT 10
#define BLE 11
#define BGE 12
#define JAL 13
#define LW 14
#define SW 15
#define RETI 16
#define IN 17
#define OUT 18
#define HALT 19


void Extract_Variabales_from_PC(FILE* f, int  num_line, int* opcode, int* reg_rd, int* reg_rs, int* reg_rt, int* imm);
void Simulator(FILE* Memout, FILE *trace, FILE *leds, FILE *diskout, FILE *hwregtrace, FILE *regout, FILE *cycles, FILE *display);
void Instructions_0_to_13_opcode(int R[], int opcode, int rd, int rs, int rt, int PC, int* PC_next);
void Jump_to_PC(FILE* f, int PC);
void Instructions_lw_sw(int R[], int opcode, int rd, int rs, int rt, int PC, int* PC_next, FILE* Memout);
void IO_Instructions(FILE* hwregtrace, FILE* leds, FILE* diskout, FILE* memout, int opcode, int R[], int IORegister[], int rd, int rs, int rt, int* PC_next, int clock_cycle);
void Copy_Text_File(FILE* source, FILE* target);
void read_write_to_disk(FILE* diskout, FILE* memout, int diskcmd, int disksector, int diskbuffer);
void print_trace(FILE* trace, int PC, int opcode, int rd, int rs, int rt, int imm, int R[]);
void print_leds(FILE *leds, int clock_cycle, int new_leds);
void print_regout(FILE *regout, int R[]);


#endif #pragma once
