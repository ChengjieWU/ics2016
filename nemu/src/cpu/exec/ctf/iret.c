#include "cpu/exec/helper.h"

uint32_t swaddr_read(swaddr_t, size_t, uint8_t);

make_helper(iret) {
	//if (cpu.PE == 0) {
		cpu.eip = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		cpu.sreg[1]._16 = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		cpu.eflags = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		return 0;
	//}
}
