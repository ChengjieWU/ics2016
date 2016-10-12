#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_,SUFFIX)) {
	swaddr_t jmpdisp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("je %x", cpu.eip + 1 + DATA_BYTE + jmpdisp);
	cpu.eip += jmpdisp;
	return 1 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
