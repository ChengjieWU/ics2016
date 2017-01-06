#include "cpu/exec/template-start.h"
#define instr cwt

make_helper(concat(cwt_, SUFFIX)) {
#if DATA_BYTE == 2
	uint32_t sym = cpu.gpr[0]._8[0] >> 7;
	if (sym) cpu.gpr[0]._16 = 0xff00 + (uint16_t)cpu.gpr[0]._8[0];
	else cpu.gpr[0]._16 = (uint16_t) cpu.gpr[0]._8[0];
#endif
#if DATA_BYTE == 4
	uint32_t sym = cpu.gpr[0]._16 >> 15;
	if (sym) cpu.gpr[0]._32 = 0xffff0000 + (uint32_t)cpu.gpr[0]._16;
	else cpu.gpr[0]._32 = (uint32_t)cpu.gpr[0]._16;
#endif
	print_asm("cwt");
	return 1;
}

#include "cpu/exec/template-end.h"
