#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "cpu.h"
#include "mmu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { es, cs, ss, ds, fs, gs };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];
	
		struct 
		{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	/* Do NOT change the order of the GPRs' definitions. */
	};

	swaddr_t eip;
	union {
		uint32_t eflags;
		struct {
			unsigned CF : 1;
			unsigned : 1;
			unsigned PF : 1;
			unsigned : 3;
			unsigned ZF : 1;
			unsigned SF : 1;
			unsigned : 1;
			unsigned IF : 1;
			unsigned DF : 1;
			unsigned OF : 1;
			unsigned : 4;
			unsigned : 8;
			unsigned : 8;
		};
	};
	
	struct {
		uint16_t limit;
		uint32_t base;
	} gdtr;

	struct {
		uint16_t limit;
		uint32_t base;
	} idtr;

	struct{
		union {
			struct {
				unsigned RPL : 3;
				unsigned TI : 1;
				unsigned INDEX : 12;
			};
			uint16_t _16;
		};
		SegDesc cache;
	} sreg[6];
	
	CR0 cr0;
	CR3 cr3;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
