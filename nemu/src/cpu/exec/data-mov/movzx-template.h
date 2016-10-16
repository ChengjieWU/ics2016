#include "cpu/exec/template-start.h"

#define instr movzx

make_helper(concat(movzx_brm2r_, SUFFIX)) {
	int len = concat(decode_brm2r_, SUFFIX)(eip + 1);
	DATA_TYPE zexp = (DATA_TYPE) op_src->val;
	OPERAND_W(op_dest, zexp);
	print_asm_template2();
	return 1 + len;
}	

#if DATA_BYTE == 4
make_helper(movzx_wrm2r) {
	int len = decode_wrm2r(eip + 1);
	DATA_TYPE zexp = (DATA_TYPE) op_src->val;
	OPERAND_W(op_dest, zexp);
	print_asm_template2();
	return 1 + len;
}
#endif

#include "cpu/exec/template-end.h"
