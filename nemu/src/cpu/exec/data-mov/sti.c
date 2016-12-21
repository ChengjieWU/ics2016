#include "cpu/exec/helper.h"

make_helper(sti) {
	print_asm("sti");
	cpu.IF = 1;
	return 1;
}
