#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_,SUFFIX)) {
	swaddr_t cur_addr;
#if DATA_BYTE == 2
	cur_addr = (eip + 1 + DATA_BYTE + instr_fetch(eip + 1, DATA_BYTE)) & 0x0000ffff;
#else
	concat(decode_si_, SUFFIX)(eip + 1);
	cur_addr = (swaddr_t)(eip + 1 + DATA_BYTE + op_src->simm);
#endif	
	print_asm("jmp %x", cur_addr);
	cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
