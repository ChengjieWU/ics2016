#include "cpu/exec/template-start.h"

#define instr sub


static void do_execute() {
	DATA_TYPE minuend = op_dest->val;
	DATA_TYPE subtrahend = op_src->val;	
	DATA_TYPE difference = minuend - subtrahend;
	OPERAND_W(op_dest, difference);
	print_asm_template2();
}


make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)




#include "cpu/exec/template-end.h"
