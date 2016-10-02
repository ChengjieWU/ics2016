#include "cpu/exec/helper.h"

make_helper(ret_near) {
	cpu.esp = cpu.ebp + 4;
	cpu.ebp = swaddr_read(cpu.ebp, 4);
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	return 0;
}
