#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_, SUFFIX)) {
	REG(R_EAX) = MEM_R(cpu.esi);
	if (cpu.DF == 0) cpu.esi += DATA_BYTE;
	else cpu.esi -= DATA_BYTE;
#if DATA_BYTE == 1
	print_asm("lodsb %%ds:(%%esi),AL");
#endif
#if	DATA_BYTE == 2
	print_asm("lodsw %%ds:(%%esi),AX");
#endif
#if DATA_BYTE == 4
	print_asm("lodsl %%ds:(%%esi),EAX");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
