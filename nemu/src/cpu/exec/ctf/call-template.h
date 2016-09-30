#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_rel_, SUFFIX)) {
	REG(R_ESP) = REG(R_ESP) - 4;
	MEM_W(REG(R_ESP), eip + 1 + DATA_BYTE);
	swaddr_t reldisp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("call %x", eip + 1 + DATA_BYTE + reldisp);
	eip = eip + reldisp;
	return 1 + DATA_BYTE;
}


#include "cpu/exec/template-end.h"
