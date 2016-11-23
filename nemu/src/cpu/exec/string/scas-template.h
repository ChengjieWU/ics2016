#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX)) {
	DATA_TYPE oprand_1 = MEM_R(cpu.edi, 0);
	DATA_TYPE oprand_2 = REG(R_EAX);
	DATA_TYPE result_cmp = oprand_2 - oprand_1;
	//ZF
	cpu.ZF = !result_cmp;
	//SF
	cpu.SF = MSB(result_cmp);
	//OF
	DATA_TYPE m1 = 1 << (8 * DATA_BYTE - 1);
	if (oprand_1 == m1) {
		cpu.OF = !MSB(oprand_2);
	}
	else {
		DATA_TYPE neg_op1 = (~oprand_1) + 1;
		if (MSB(neg_op1) == MSB(oprand_2) && MSB(oprand_2) != MSB(result_cmp)) cpu.OF = 1;
		else cpu.OF = 0;
	}
	//CF
	if (oprand_1 > oprand_2) cpu.CF = 1;
	else cpu.CF = 0;
	//PF
	cpu.PF = 1;
	DATA_TYPE dpf = result_cmp;
	int loop_i = 0;
	for (; loop_i < 8; loop_i++) {
		if ((dpf & 0x1) == 1) cpu.PF = ~cpu.PF;
		dpf = dpf >> 1;
	}
	//IF, DF were not affected

	if (cpu.DF == 0) cpu.edi += DATA_BYTE;
	else cpu.edi -= DATA_BYTE;
#if DATA_BYTE == 1
	print_asm("scasb %%es:(%%edi),%%al");
#endif
#if	DATA_BYTE == 2
	print_asm("scasw %%es:(%%edi),%%ax");
#endif
#if DATA_BYTE == 4
	print_asm("scasl %%es:(%%edi),%%eax");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
