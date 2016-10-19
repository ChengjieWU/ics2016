#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	REG(R_EDI) = REG(R_ESI);
#if DATA_BYTE == 2
	print_asm("movsw %%ds:(%%esi),%%es:(%%edi)");
#else
	print_asm("movsl %%ds:(%%esi),%%es:(%%edi)");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
