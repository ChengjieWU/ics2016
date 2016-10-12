#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE oprand1 = op_src->val;
	DATA_TYPE oprand2 = op_src2->val;
	DATA_TYPE result_pa = oprand1 & oprand2;		//register is in src2
	//OF
	cpu.OF = 0;
	//CF
	cpu.CF = 0;
	//ZF
	cpu.ZF = !result_pa;
	//SF
	if (MSB(result_pa)) cpu.SF = 1;
	else cpu.SF = 0;
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = result_pa;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//DF, IF were not affected
	print_asm_template4();
}

make_instr_helper(i_a)
make_instr_helper(r_rm)
make_instr_helper(i_rm)

#include "cpu/exec/template-end.h"
