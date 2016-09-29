#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	reg_l(R_ESP) = reg_l(R_ESP) - 4;
	return 5; 
}
