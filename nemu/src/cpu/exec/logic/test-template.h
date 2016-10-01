#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	uint32_t result_pa = op_src->val & op_src2->val;		//register is in src2
	cpu.OF = 0;
	cpu.CF = 0;
	if (result_pa == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if (result_pa >> (8 * DATA_BYTE - 1) == 1) cpu.SF = 1;
	else cpu.SF = 0;
	cpu.PF = 1;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((result_pa & 0x00000001) == 1) cpu.PF = ~cpu.PF;
		result_pa = result_pa >> 1;
	}
	print_asm_template4();
}

make_instr_helper(i_a)
make_instr_helper(r_rm)
make_instr_helper(i_rm)

#include "cpu/exec/template-end.h"
