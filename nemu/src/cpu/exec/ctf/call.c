#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	reg_l(R_ESP) = reg_l(R_ESP) - 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + 5);
	swaddr_t reldisp = instr_fetch(cpu.eip + 1, 4);
	reldisp += 5;
	print_asm("call %x", reldisp);
	return 5; 
}
