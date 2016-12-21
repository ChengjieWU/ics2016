#include "device/port-io.h"

#include "cpu/exec/template-start.h"
#define instr in
/*
make_helper(concat(in_i_, SUFFIX)) {
	print_asm("in");
	uint16_t addr = instr_fetch(eip + 1, 1);
	REG(R_EAX) = pio_read(addr, DATA_BYTE);
	return 2;
}

make_helper(concat(in_d_, SUFFIX)) {
	print_asm("in");
	uint16_t addr = cpu.gpr[2]._16;
	REG(R_EAX) = pio_read(addr, DATA_BYTE);
	return 1;
}
*/

make_helper(concat(in_i_, SUFFIX)) {
	uint32_t len = decode_i_b(eip + 1);
	REG(R_EAX) = pio_read(op_src->val, DATA_BYTE);
	print_asm("in");
	return 1 + len;
}

make_helper(concat(in_d_, SUFFIX)) {
	REG(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);
	print_asm("in");
	return 1;
}

#include "cpu/exec/template-end.h"
