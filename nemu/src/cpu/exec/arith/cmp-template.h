#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE oprand_1 = op_src->val;
	DATA_TYPE oprand_2 = op_src2->val;
	DATA_TYPE result_cmp = oprand_2 - oprand_1;
	//ZF
	cpu.ZF = !result_cmp;
	//SF
	cpu.SF = MSB(result_cmp);
	//OF
	DATA_TYPE m1 = 1 << (8 * DATA_BYTE - 1);
	printf("%x %x %x\n", oprand_2, oprand_1, result_cmp);
	if (oprand_1 == m1) {
		if (oprand_2 != 0 && MSB(oprand_2) == 0) cpu.OF = 0;
		else cpu.OF = 1;
	}
	else {
		DATA_TYPE neg_op1 = (~oprand_1) + 1;
		printf("%x\n", neg_op1);
		printf("%d %d\n\n", MSB(neg_op1)==MSB(oprand_2), MSB(oprand_2)!=MSB(result_cmp));
		if (MSB(neg_op1) == MSB(oprand_2) && MSB(oprand_2) != MSB(result_cmp)) cpu.OF = 1;
		else cpu.OF = 0;
	}
	//CF
	if (oprand_1 > oprand_2) cpu.CF = 1;
	else cpu.CF = 0;
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = result_cmp;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//IF, DF were not affected
	print_asm_template4();
}


make_instr_helper(i_a)
make_instr_helper(i_rm)
make_instr_helper(r_rm)
make_instr_helper(rm_r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si_rm)
#endif

#include "cpu/exec/template-end.h"
