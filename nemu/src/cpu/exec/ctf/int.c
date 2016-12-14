#include "cpu/exec/helper.h"

void raise_intr(uint8_t NO);

make_helper(int_imm) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	raise_intr(imm);
	return 2;
}
