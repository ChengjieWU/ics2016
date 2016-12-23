#include "cpu/exec/template-start.h"
#define instr bt

make_helper(concat(bt_i2rm_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	len += decode_i_b(eip + 1 + len);
	uint32_t mask = 1;
	mask = mask << (op_src->val % DATA_BYTE);
	if (op_dest->val & mask) cpu.CF = 1;
	else cpu.CF = 0;
	return 1 + len;
}

make_helper(concat(bt_r2rm_, SUFFIX)) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	uint32_t mask = 1;
	mask = mask << (op_src->val % (8 * DATA_BYTE));
	if (op_dest->val & mask) cpu.CF = 1;
	else cpu.CF = 0;
	return 1 + len;
}

/*
make_helper(concat(bt_r2rm_, SUFFIX)) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (op_dest->type == OP_TYPE_REG)
	{
		uint32_t offset = op_src->val % (DATA_BYTE * 8);
		cpu.CF = (op_dest->val >> offset) & 1;
	}
	else
	{
		uint32_t addr = op_src->val / (DATA_BYTE * 8) + op_dest->addr;
		uint32_t offset = op_src->val % (DATA_BYTE * 8);
		cpu.CF = (swaddr_read(addr, DATA_BYTE, op_dest->sreg) >> offset) & 1;
	}
	return 1 + len;
}
*/

#include "cpu/exec/template-end.h"
