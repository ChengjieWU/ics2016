#include "cpu/exec/template-start.h"
#include <stdio.h>
#define instr lgdt

/*make_helper(concat(lgdt_, SUFFIX)) {
	int len = decode_rm2r_l(eip + 1);
	printf("%x\n", op_src->val);
	cpu.gdtr.limit = lnaddr_read(op_src->val, 2);
	if (DATA_BYTE == 2) cpu.gdtr.base = lnaddr_read(op_src->val + 2, 3);
	else cpu.gdtr.base = lnaddr_read(op_src->val + 2, 4);
	print_asm("lgdt 0x%hx 0x%x", cpu.gdtr.limit, cpu.gdtr.base);
	return len + 1;
}*/


make_helper(concat(lgdt_, SUFFIX)) {
	op_src->val = instr_fetch(eip + 2, 4);
	if (DATA_BYTE == 2) cpu.gdtr.base = lnaddr_read(op_src->val + 2, 3);
	else cpu.gdtr.base = lnaddr_read(op_src->val + 2, 4);
	print_asm("lgdt 0x%hx 0x%x", cpu.gdtr.limit, cpu.gdtr.base);
	return 6;
}



#include "cpu/exec/template-end.h"
