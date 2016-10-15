#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_,SUFFIX)) {
	cpu.esp = cpu.ebp;
	REG(R_EBP) = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
