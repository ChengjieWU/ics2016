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

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_,SUFFIX)) {
	int len = concat(decode_rm_,SUFFIX)(eip + 1);
	swaddr_t cur_addr;
#if DATA_BYTE == 2
	cur_addr = (swaddr_t)op_src->val & 0x0000ffff;
#else
	cur_addr = (swaddr_t)op_src->val;
#endif
	print_asm("jmp %s", op_src->str);
	cpu.eip = cur_addr - (1 + len);
	return 1 + len;
}
#endif

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_l_, SUFFIX)) {
	swaddr_t offset = instr_fetch(eip + 1, DATA_BYTE);
	uint16_t cs_new = instr_fetch(eip + 5, 2);
	cpu.eip = offset - 7;
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
#endif
	cpu.sreg[1]._16 = cs_new;
	cpu.sreg[1].cache.base_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 2, 2);
	cpu.sreg[1].cache.base_23_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 4, 1);
	cpu.sreg[1].cache.base_31_24 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 7, 1);
	cpu.sreg[1].cache.limit_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX, 2);
	cpu.sreg[1].cache.limit_19_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 6, 1) & 0xf;

	print_asm("ljmp $%hx,$%x", cs_new, offset);
	return DATA_BYTE + 3;
}
#endif

#include "cpu/exec/template-end.h"
