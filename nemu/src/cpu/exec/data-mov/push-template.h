#include "cpu/exec/template-start.h"

#define instr push

make_helper(concat(push_r_, SUFFIX)) {
	cpu.esp -= DATA_BYTE;
	int regnum = instr_fetch(eip, 1) & 0x7;
	MEM_W(cpu.esp, REG(regnum));
	print_asm("push %%%s", REG_NAME(regnum));
	return 1;
}

make_helper(concat(push_m_, SUFFIX)) {
	cpu.esp -= DATA_BYTE;
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	MEM_W(cpu.esp, op_src->val);
	print_asm("push %s", op_src->str);
	return 1 + len;	
}

#include "cpu/exec/template-end.h"
