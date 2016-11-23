#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	MEM_W(cpu.edi, MEM_R(cpu.esi, 3), 0);
	if (cpu.DF == 0) {
		cpu.esi += DATA_BYTE;
		cpu.edi += DATA_BYTE;
	}
	else {
		cpu.esi -= DATA_BYTE;
		cpu.edi -= DATA_BYTE;
	}
#if DATA_BYTE == 1
	print_asm("movsb %%ds:(%%esi),%%es:(%%edi)");
#endif
#if	DATA_BYTE == 2
	print_asm("movsw %%ds:(%%esi),%%es:(%%edi)");
#endif
#if DATA_BYTE == 4
	print_asm("movsl %%ds:(%%esi),%%es:(%%edi)");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
