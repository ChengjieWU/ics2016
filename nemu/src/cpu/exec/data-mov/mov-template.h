#include "cpu/exec/template-start.h"
#define instr mov

static void do_execute() {
	printf("//%x//", op_dest->sreg);
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), 3);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, 3);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4
make_helper(movsr) {
	int len = decode_r2rm_l(eip + 1);
	REG(op_src->reg) = cpu.cr0.val;
	print_asm("mov %%cr0,%s", op_src->str);
	return len + 1;
}

make_helper(movsrr) {
	int len = decode_r2rm_l(eip + 1);
	cpu.cr0.val = REG(op_src->reg);
	print_asm("mov %s,%%cr0", op_src->str);
	return len + 1;
}
#endif

#if DATA_BYTE == 2
make_helper(movrm2s) {
	int len = decode_r2rm_w(eip + 1);
	cpu.sreg[op_src->reg]._16 = op_dest->val & 0xffff;
	switch (op_src->reg) {
		case 0: print_asm("mov %s,%%es", op_dest->str); break;
		case 1: print_asm("mov %s,%%cs", op_dest->str); break;
		case 2: print_asm("mov %s,%%ss", op_dest->str); break;
		case 3: print_asm("mov %s,%%ds", op_dest->str); break;
		default: assert(0);
	}
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
