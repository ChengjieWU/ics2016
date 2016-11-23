#include "cpu/exec/template-start.h"

#define instr lgdt

make_helper(concat(lgdt_, SUFFIX)) {
	uint16_t t1 = instr_fetch(eip + 1, 2);
	uint32_t t2;
	//if (DATA_BYTE == 2) t2 = instr_fetch(eip + 3, 3) & 0x00111111;
	//else t2 = instr_fetch(eip + 3, 4);
	t2 = instr_fetch(eip + 3, 4) & 0x00111111;
	cpu.gdtr.limit = t1;
	cpu.gdtr.base = t2;
	//int ret;
	//if (DATA_BYTE == 2) ret = 6;
	//else ret = 7;
	print_asm("lgdt 0x%hx 0x%x", t1, t2);
	//return ret;
	return 6;
}




#include "cpu/exec/template-end.h"
