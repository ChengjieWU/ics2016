#include "cpu/exec/template-start.h"

#define instr jcc

make_helper(concat(jcc_e_,SUFFIX)) {
	swaddr_t jccdisp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("je %x", cpu.eip + jccdisp + 1 + DATA_BYTE);
	if (cpu.ZF == 1) cpu.eip += jccdisp;
	return 1 + DATA_BYTE;	
}

make_helper(concat(jcc_be_,SUFFIX)) {
	swaddr_t jccdisp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jbe %x", cpu.eip + 1 + DATA_BYTE + jccdisp);
	if (cpu.ZF == 1 || cpu.CF == 1) cpu.eip += jccdisp;
	return 1 + DATA_BYTE;
}


#include "cpu/exec/template-end.h"
