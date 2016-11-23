#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_rel_, SUFFIX)) {
	REG(R_ESP) -=  DATA_BYTE;	
	if (DATA_BYTE == 4) {
		MEM_W(REG(R_ESP), eip + 1 + DATA_BYTE, 1);
		swaddr_t reldisp = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("call %x", eip + reldisp + 1 + DATA_BYTE);
		cpu.eip = eip + reldisp;
	}
	else if (DATA_BYTE == 2) {
		MEM_W(REG(R_ESP), (eip + 1 + DATA_BYTE) & 0x0000ffff, 1);
		swaddr_t reldisp = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("call %x", (eip + reldisp + 1 + DATA_BYTE) & 0x0000ffff);
		cpu.eip = (eip + reldisp) & 0x0000ffff;
	}	//eip cannot write to cpu.eip
	return 1 + DATA_BYTE;
}

make_helper(concat(call_rm_, SUFFIX)) {
	cpu.esp -= DATA_BYTE;
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	MEM_W(cpu.esp, eip + 1 + len, 1);
	cpu.eip = op_src->val - 1 - len;
	print_asm("call %s", op_src->str);
	return 1 + len;
}


#include "cpu/exec/template-end.h"
