#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_,SUFFIX)) {
	REG(R_ESP) = REG(R_EBP);
	REG(R_EBP) = MEM_R(REG(R_ESP));
	REG(R_ESP) += DATA_BYTE;
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
