#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	REG(R_EDI) = REG(R_ESI);
	return 1;
}

#include "cpu/exec/template-end.h"
