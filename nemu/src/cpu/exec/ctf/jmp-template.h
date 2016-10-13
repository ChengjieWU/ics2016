#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_,SUFFIX)) {
	DATA_TYPE cur_lo = (DATA_TYPE)(eip + 1 + DATA_BYTE) + instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE ze = ~0x0;
	swaddr_t cur_addr = ((eip + 1 + DATA_BYTE) & (~(swaddr_t)ze)) + cur_lo;
	print_asm("jmp %x", cur_addr);
	cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
