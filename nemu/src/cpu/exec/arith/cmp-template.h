#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	uint32_t oprand_1 = op_src->val;
	uint32_t oprand_2 = op_src2->val;
	uint32_t result_cmp = oprand_2 - oprand_1;
	//ZF
	if (result_cmp == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	//SF
	if (((result_cmp >> (8 * DATA_BYTE - 1)) & 0x1) == 1) cpu.SF = 1;
	else cpu.SF = 0;
	//OF
	if (((oprand_1 >> (8 * DATA_BYTE - 1)) & 0x1) == ((oprand_2 >> (8 * DATA_BYTE - 1)) & 0x1) 
		&& ((oprand_1 >> (8 * DATA_BYTE - 1)) & 0x1) != ((result_cmp >> (8 * DATA_BYTE - 1)) & 0x1))
		cpu.OF = 1;
	else cpu.OF = 0;
	//CF
	if (oprand_1 > oprand_2) cpu.CF = 1;
	else cpu.CF = 0;
	//PF
	cpu.PF = 1;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((result_cmp & 0x1) == 1) cpu.PF = ~cpu.PF;
		result_cmp = result_cmp >> 1;
	}
	//IF, DF were not affected
	print_asm_template4();
}


make_instr_helper(i_a)
make_instr_helper(i_rm)
make_instr_helper(r_rm)
make_instr_helper(rm_r)


#include "cpu/exec/template-end.h"
