#include "cpu/exec/template-start.h"

#define instr push

make_helper(concat(push_r_, SUFFIX)) {
	REG(R_ESP) -= DATA_BYTE;
	uint32_t regnum = instr_fetch(eip, 1) & 0x07;
	MEM_W(REG(R_ESP), REG(regnum));
	print_asm("push %%%s", REG_NAME(regnum));
	return 1;
}


#include "cpu/exec/template-end.h"
