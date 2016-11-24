#include "common.h"

/* @uint32_t dram_read(hwaddr_t, size_t); */
/* @void dram_write(hwaddr_t, size_t, uint32_t); */
uint32_t Cache_1_read(hwaddr_t, size_t);
void Cache_1_write(hwaddr_t, size_t, uint32_t);
uint32_t Cache_2_read(hwaddr_t, size_t);
void Cache_2_write(hwaddr_t, size_t, uint32_t);

bool cpu_cr0_protect_enable();
int cpu_sreg_index(uint8_t);
unsigned cpu_gdtr_base();
lnaddr_t cpu_sreg_cache_base(uint8_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	/* @return dram_read(addr, len) & (~0u >> ((4 - len) << 3)); */
	return Cache_1_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	/* @dram_write(addr, len, data); */
	Cache_1_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
	lnaddr_t ret_addr = addr;
	if (cpu_cr0_protect_enable()) {
		ret_addr += cpu_sreg_cache_base(sreg);
		/*int index = cpu_index(sreg);
		unsigned buf1 = lnaddr_read(cpu_gdtr_base() + 8 * index, 4); 
		unsigned buf2 = lnaddr_read(cpu_gdtr_base() + 8 * index + 4, 4);
		ret_addr += (buf1 >> 16) | ((buf2 & 0xff) << 16) | (buf2 & 0xff000000);*/
	}
	return ret_addr;	
}

/*
uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}*/

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	assert(sreg == 0 || sreg == 1 || sreg == 2 || sreg == 3 || sreg == 4 || sreg == 5);
	return lnaddr_read(lnaddr, len);
}

/*
void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}*/

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_write(lnaddr, len, data);
}

