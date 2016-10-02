#include "cpu/exec/template-start.h"

#define instr pop

make_helper(concat(pop_r_, SUFFIX)) {
	int regnum = instr_fetch(eip, 1) & 0x7;
	REG(regnum) = MEM_R(REG(R_ESP));
	REG(R_ESP) -= DATA_BYTE;
	return 1;
}

make_helper(concat(pop_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	MEM_W(op_src->addr, MEM_R(REG(R_ESP)));
	REG(R_ESP) -= DATA_BYTE;
	return 1 + len;
}

#include "cpu/exec/template-end.h"
