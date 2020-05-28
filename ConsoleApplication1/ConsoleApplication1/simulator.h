#ifndef	SIMULATOR_H
#define SIMULATOR_H

#define MAX_REG 16
#define INSTRUCTION_LEN 10
#define MASK_REG(R) R&0xFFF//save only the 12 low bits 

#define	BEQ 7
#define BNE 8
#define BLT 9
#define BGT 10
#define BLE 11
#define BGE 12
#define JAL 13

void Extract_Variabales_from_PC(FILE* f, int  num_line, int* opcode, int* reg_rd, int* reg_rs, int* reg_rt, int* imm);
void Simulator(FILE* Memin);
void Branch_Jump_opcode(int R[], int opcode, int rd, int rs, int rt, int imm, int PC, int* PC_next);
void Jump_to_PC(FILE* f, int PC);


#endif 