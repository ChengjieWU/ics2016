#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE minuend = op_dest->val;
	DATA_TYPE subtrahend = op_src->val;	
	DATA_TYPE difference = minuend - subtrahend;
	OPERAND_W(op_dest, difference);

	//ZF
	cpu.ZF = !difference;
	//SF
	cpu.SF = MSB(difference);
	//OF
	DATA_TYPE m1 = 1 << (8 * DATA_BYTE - 1);
	if (subtrahend == m1) {
		cpu.OF = !MSB(minuend);
	}
	else {
		DATA_TYPE neg_subtrahend = (~subtrahend) + 1;
		if (MSB(minuend) == MSB(neg_subtrahend) && MSB(minuend) != MSB(difference)) cpu.OF = 1;
		else cpu.OF = 0;
	}
	//CF
	if (subtrahend > minuend) cpu.CF = 1;
	else cpu.CF = 0;
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = difference;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//IF, DF were not affected
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
