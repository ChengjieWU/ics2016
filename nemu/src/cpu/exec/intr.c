#include "nemu.h"
#include "mmu.h"
void swaddr_write(swaddr_t, size_t, uint32_t, uint8_t);

#include <setjmp.h>
extern jmp_buf jbuf;
void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *    * That is, use ``NO'' to index the IDT.
	 *       */
	/* Jump back to cpu_exec() */

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eflags, 2);
	cpu.esp -= 2;
	swaddr_write(cpu.esp, 2, cpu.sreg[1]._16, 2);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, 2);
	

	longjmp(jbuf, 1);
}
