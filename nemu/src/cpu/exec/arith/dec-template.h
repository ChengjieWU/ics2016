#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	//ZF
	cpu.ZF = !result;
	//SF
	cpu.SF = MSB(result);
	//OF
	DATA_TYPE help = 1;
	help = help << (8 * DATA_BYTE - 1);
	cpu.OF = (op_src->val == help);
	//CF
	cpu.CF = (op_src->val == 0);
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = result;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//IF, DF were not affected

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
