#include "cpu/exec/template-start.h"

#define instr jcc

make_helper(concat(jcc_e_,SUFFIX)) {
	swaddr_t jccdisp = instr_fetch(eip + 1, DATA_BYTE);
	if (DATA_BYTE == 4) {
		print_asm("je 0x%x", cpu.eip + jccdisp + 1 + DATA_BYTE);
		if (cpu.ZF == 1) cpu.eip += jccdisp;
	}
	else if (DATA_BYTE == 2) {
		print_asm("je 0x%x", (cpu.eip + jccdisp + 1 + DATA_BYTE) & 0x0000ffff);
		if (cpu.ZF == 1) cpu.eip = (cpu.eip + jccdisp) & 0x0000ffff;
	}
	return 1 + DATA_BYTE;	
}

#include "cpu/exec/template-end.h"
