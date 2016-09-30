#include "cpu/exec/helper.h"

/*make_helper(call_rel32) {
	reg_l(R_ESP) = reg_l(R_ESP) - 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + 5);
	swaddr_t reldisp = instr_fetch(cpu.eip + 1, 4);
	print_asm("call %x", cpu.eip + 5 + reldisp);
	cpu.eip += reldisp;
	return 5; 
}*/


#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

make_helper_v(call_rel)
