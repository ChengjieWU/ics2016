#include "cpu/exec/helper.h"

make_helper(ret_near) {
	cpu.eip = swaddr_read(cpu.esp, 4) - 1;
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}

make_helper(ret_near_imm) {
	cpu.eip = swaddr_read(cpu.esp, 4) - 3;
	cpu.esp += 4;
	uint16_t imm = instr_fetch(eip + 1, 2);
	cpu.esp += imm;
	print_asm("ret $0x%x", imm);
	return 3;
}
