#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	DATA_TYPE dest = op_dest->val;
	DATA_TYPE src = op_src->val;
	DATA_TYPE sum = dest + src;
	OPERAND_W(op_dest, sum);

	//ZF
	cpu.ZF = !sum;
	//SF
	cpu.SF = MSB(sum);
	//OF
	if (MSB(src) == MSB(dest) && MSB(dest) != MSB(sum)) cpu.OF = 1;
	else cpu.OF = 0;
	//CF
	if (sum < dest) cpu.CF = 1;
	else cpu.CF = 0;
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = sum;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//IF, DF were not affectd
	
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
