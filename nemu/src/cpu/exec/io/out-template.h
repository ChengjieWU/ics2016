#include "device/port-io.h"

#include "cpu/exec/template-start.h"
#define instr out
/*
make_helper(concat(out_i_, SUFFIX)) {
	print_asm("out");
	uint16_t addr = instr_fetch(eip + 1, 1);
	pio_write(addr, DATA_BYTE, (uint32_t)REG(R_EAX));
	return 2;
}

make_helper(concat(out_d_, SUFFIX)) {
	print_asm("out");
	uint16_t addr = cpu.gpr[2]._16;
	pio_write(addr, DATA_BYTE, (uint32_t)REG(R_EAX));
	return 1;
}
*/
make_helper(concat(out_i_, SUFFIX)) {
	uint32_t len = decode_i_b(eip + 1);
	pio_write(op_src->val, DATA_BYTE, REG(R_EAX));
	return 1 + len;
}

make_helper(concat(out_d_, SUFFIX)) {
	pio_write(reg_w(R_DX), DATA_BYTE, REG(R_EAX));
	return 1;
}



#include "cpu/exec/template-end.h"
