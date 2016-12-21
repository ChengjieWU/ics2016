#include "device/port-io.h"

#include "cpu/exec/template-start.h"
#define instr in

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

#include "cpu/exec/template-end.h"
