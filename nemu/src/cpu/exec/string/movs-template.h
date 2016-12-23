#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	if (DATA_BYTE == 1)
	/*if (cpu.esi == 0xc0102f83)	once dubug
	{
		printf("%x, %x", swaddr_read(cpu.esi, 4, 0), swaddr_read(cpu.edi, 4, 0));
	}*/
	//uint32_t temp = swaddr_read(cpu.esi, 1, 0);
	//swaddr_write(cpu.edi, 1, temp, 0);
	//MEM_W(cpu.edi, cpu.esi, 0);
	{MEM_W(cpu.edi, MEM_R(cpu.esi, 3), 0);
	if (cpu.DF == 0) {
		cpu.edi += 1;
		cpu.esi += 1;
	}
	else {
		cpu.edi -= 1;
		cpu.esi -= 1;
	}
	}
	else {
	MEM_W(REG(R_EDI), MEM_R(REG(R_ESI), 3), 0);
	if (cpu.DF == 0) {
		REG(R_ESI) += DATA_BYTE;
		REG(R_EDI) += DATA_BYTE;
	}
	else {
		REG(R_ESI) -= DATA_BYTE;
		REG(R_EDI) -= DATA_BYTE;
	}}
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
