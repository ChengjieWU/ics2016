#include "cpu/exec/template-start.h"

#define instr movsx

make_helper(concat(movsx_brm2r_, SUFFIX)) {
	int len = concat(decode_brm2r_, SUFFIX)(eip + 1);
	DATA_TYPE zexp = (DATA_TYPE) op_src->val;
	DATA_TYPE help = 0;
	if (zexp & 0x80) help = (~help >> 8) << 8;
	DATA_TYPE_S sexp = (DATA_TYPE_S)(zexp | help);
	OPERAND_W(op_dest, (DATA_TYPE)sexp);
	print_asm_template2();
	return 1 + len;
}	

#if DATA_BYTE == 4
make_helper(movsx_wrm2r) {
	int len = decode_wrm2r(eip + 1);
	DATA_TYPE zexp = (DATA_TYPE) op_src->val;
	DATA_TYPE help = 0;
	if (zexp & 0x8000) help = (~help >> 16) << 16;
	DATA_TYPE_S sexp = (DATA_TYPE_S)(zexp | help);
	OPERAND_W(op_dest, (DATA_TYPE)sexp);
	print_asm_template2();
	return 1 + len;
}
#endif

#include "cpu/exec/template-end.h"
