#include "cpu/exec/helper.h"

make_helper(ret_near) {
	cpu.eip = swaddr_read(cpu.esp, 4) - 1;
	cpu.eip += 4;
	return 1;
}
