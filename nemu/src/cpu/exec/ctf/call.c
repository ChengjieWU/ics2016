#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	reg_l(R_ESP) = reg_l(R_ESP) - 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + 5);
	swaddr_t reldisp = instr_fetch(cpu.eip + 1, 4);
	print_asm("call %x", 5 + reldisp);
	return 5 + reldisp; 
}
