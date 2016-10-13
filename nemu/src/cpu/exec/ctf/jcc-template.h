#include "cpu/exec/template-start.h"

#define instr jcc

make_helper(concat(jcc_e_,SUFFIX)) {
	DATA_TYPE jccdisp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("je %x", cpu.eip + jccdisp + 1 + DATA_BYTE);
	if (cpu.ZF == 1) cpu.eip += jccdisp;
	return 1 + DATA_BYTE;	
}

make_helper(concat(jcc_be_,SUFFIX)) {
	eip += (1 + DATA_BYTE);
	DATA_TYPE cur_lo = (DATA_TYPE)eip + instr_fetch(eip - DATA_BYTE, DATA_BYTE);
	DATA_TYPE ze = ~0x0;
	swaddr_t cur_addr = (eip & (~(swaddr_t)ze)) + cur_lo;
	print_asm("jbe %x", cur_addr);
	if (cpu.ZF == 1 || cpu.CF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}


#include "cpu/exec/template-end.h"
