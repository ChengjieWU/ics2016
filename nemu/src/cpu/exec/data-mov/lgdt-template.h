#include "cpu/exec/template-start.h"

#define instr lgdt

make_helper(concat(lgdt_, SUFFIX)) {
	uint16_t t1 = instr_fetch(eip + 1, 2);
	uint32_t t2;
	t2 = instr_fetch(eip + 3, 4) & 0x00111111;
	cpu.gdtr.limit = t1;
	cpu.gdtr.base = t2;
	print_asm("lgdt 0x%hx 0x%x", t1, t2);
	return 6;
}




#include "cpu/exec/template-end.h"