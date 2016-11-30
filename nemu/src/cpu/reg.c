#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

bool cpu_cr0_protect_enable()
{
	if (cpu.cr0.protect_enable != 0) return true;
	else return false;
}

int cpu_sreg_index(uint8_t sreg)
{
	return cpu.sreg[sreg].INDEX;	
}

unsigned cpu_gdtr_base()
{
	return cpu.gdtr.base;
}

lnaddr_t cpu_sreg_cache_base(uint8_t sreg)
{
	return cpu.sreg[sreg].cache.base_15_0 | (cpu.sreg[sreg].cache.base_23_16 << 16) | (cpu.sreg[sreg].cache.base_31_24 << 24);
}

void init_cs_cache() {
	cpu.sreg[1].cache.base_15_0 = 0;
	cpu.sreg[1].cache.base_23_16 = 0;
	cpu.sreg[1].cache.base_31_24 = 0;
	cpu.sreg[1].cache.limit_15_0 = 0xffff;
	cpu.sreg[1].cache.limit_19_16 = 0xf;
}

bool cpu_cr0_paging()
{
	if (cpu.cr0.paging != 0) return true;
	else return false;
}

uint32_t cpu_cr3_page_directory_base()
{
	return cpu.cr3.page_directory_base;
}
