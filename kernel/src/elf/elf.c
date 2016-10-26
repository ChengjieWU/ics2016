#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;
	uint8_t buf[4096];
#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);


	//const uint32_t haha = 0x0101464c;
	//nemu_assert( *(p_magic + 2) == haha);

	/* Load each program segment */
	int loop_var = 0;
	//panic("please implement me");
	for(; loop_var < elf->e_phnum; loop_var++) {
		/* Scan the program header table, load each segment into memory */
		ph =(void*)((unsigned)elf + (unsigned)(elf->e_phoff) + (unsigned)(loop_var * elf->e_phentsize));
		if(ph->p_type == PT_LOAD) {
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			uint8_t buff[4096];
			ramdisk_read(buff, ELF_OFFSET_IN_DISK+ph->p_offset, ph->p_filesz);
			memcpy(((void *)0) + ELF_OFFSET_IN_DISK + ph->p_offset, buff, ph->p_filesz);

			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			memset(buff, 0, sizeof buff);
			memcpy(((void *)0) + ELF_OFFSET_IN_DISK + ph->p_offset + ph->p_filesz, buff, ph->p_memsz);

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
		}
	}

	nemu_assert(0);
	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
