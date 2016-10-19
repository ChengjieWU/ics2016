#include "cpu/exec/helper.h"

make_helper(cwtlcltd_w) {
	if (cpu.gpr[0]._16 >> 15) cpu.gpr[2]._16 = 0xffff;
	else cpu.gpr[2]._16 = 0x0000;
	print_asm("cwtl");
	return 1;
}

make_helper(cwtlcltd_l) {
	if (cpu.eax >> 31) cpu.edx = 0xffffffff;
	else cpu.edx = 0x00000000;
	print_asm("cltd");
	return 1;
}

make_helper_v(cwtlcltd)
