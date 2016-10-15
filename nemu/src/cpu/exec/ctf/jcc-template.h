#include "cpu/exec/template-start.h"

#define instr jcc

swaddr_t concat(next_addr_,SUFFIX)(swaddr_t eip) {
	DATA_TYPE cur_lo = (DATA_TYPE)(eip + 1 + DATA_BYTE) + instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE ze = ~0x0;
	swaddr_t cur_addr = ((eip + 1 + DATA_BYTE) & (~(swaddr_t)ze)) + cur_lo;
	return cur_addr;
}


make_helper(concat(jcc_e_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("je %x", cur_addr);
	if (cpu.ZF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;	
}

make_helper(concat(jcc_a_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("ja %x", cur_addr);
	if (cpu.CF == 0 && cpu.ZF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

make_helper(concat(jcc_ae_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jae %x", cur_addr);
	if (cpu.CF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

make_helper(concat(jcc_b_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jb %x", cur_addr);
	if (cpu.CF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;	
}

make_helper(concat(jcc_be_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jbe %x", cur_addr);
	if (cpu.ZF == 1 || cpu.CF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

make_helper(concat(jcc_ecxz_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jecxz %x", cur_addr);
	if (cpu.gpr[1]._32 == 0) cpu.eip = cur_addr - (1 + DATA_BYTE);
	return 1 + DATA_BYTE;	
}

#include "cpu/exec/template-end.h"
