#include "cpu/exec/template-start.h"

#define instr pop

make_helper(concat(pop_r_, SUFFIX)) {
	int regnum = instr_fetch(eip, 1) & 0x7;
	REG(regnum) = MEM_R(cpu.esp, 2);
	cpu.esp += DATA_BYTE;
	print_asm("pop %%%s", REG_NAME(regnum));
	return 1;
}

make_helper(concat(pop_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	MEM_W(op_src->addr, MEM_R(cpu.esp, 2), op_src->sreg);
	cpu.esp += DATA_BYTE;
	print_asm("pop %s", op_src->str);
	return 1 + len;
}

#include "cpu/exec/template-end.h"
