#include "cpu/exec/helper.h"

make_helper(ret_near) {
	cpu.esp = cpu.ebp + 4;
	printf("0x%x\n", cpu.esp);
	cpu.ebp = swaddr_read(cpu.ebp, 4);
	printf("0x%x\n", cpu.ebp);
	cpu.eip = swaddr_read(cpu.esp, 4) - 1;
	printf("0x%x\n", cpu.eip);
	cpu.esp += 4;
	printf("0x%x\n", cpu.esp);
	print_asm("ret");
	return 1;
}
