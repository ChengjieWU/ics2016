#include "cpu/exec/helper.h"

make_helper(hlt) {
	while(!cpu.INTR);	
	return 1;
}
