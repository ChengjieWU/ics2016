#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_, SUFFIX)) {
	MEM_W(cpu.edi, REG(R_EAX), 0);
	if (cpu.DF == 0) cpu.edi += DATA_BYTE;
	else cpu.edi -= DATA_BYTE;
#if DATA_BYTE == 1
	print_asm("stosb %%al,%%es:(%%edi)");
#endif
#if	DATA_BYTE == 2
	print_asm("stosw %%ax,%%es:(%%edi)");
#endif
#if DATA_BYTE == 4
	print_asm("stosl %%eax,%%es:(%%edi)");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
