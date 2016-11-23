#include "cpu/exec/template-start.h"

#define instr push

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(push_r_, SUFFIX)) {
	int regnum = instr_fetch(eip, 1) & 0x7;
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(regnum), 2);
	print_asm("push %%%s", REG_NAME(regnum));
	return 1;
}

make_helper(concat(push_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, op_src->val, 2);
	print_asm("push %s", op_src->str);
	return 1 + len;	
}
#endif

#if DATA_BYTE == 1 || DATA_BYTE == 4
make_helper(concat(push_i_, SUFFIX)) {
	cpu.esp -= 4;
	concat(decode_i_, SUFFIX)(eip + 1);
	uint32_t src = (uint32_t)op_src->val;
	swaddr_write(cpu.esp, 4,  src, 2);
	print_asm("push $0x%x", op_src->val);
	return 1 + DATA_BYTE;
}
#else
make_helper(concat(push_i_, SUFFIX)) {
	cpu.esp -= DATA_BYTE;
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	MEM_W(cpu.esp, imm, 2);
	print_asm("push $0x%x", imm);
	return 1 + DATA_BYTE;	
}
#endif

#include "cpu/exec/template-end.h"
