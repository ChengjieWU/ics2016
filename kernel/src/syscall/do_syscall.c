#include "irq.h"

#include <sys/syscall.h>

void serial_printc(char);

void add_irq_handle(int, void (*)(void));
uint32_t mm_brk(uint32_t);
int fs_ioctl(int, uint32_t, void *);
int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, size_t len);
int fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);

static void sys_brk(TrapFrame *tf) {
	tf->eax = mm_brk(tf->ebx);
}

static void sys_ioctl(TrapFrame *tf) {
	tf->eax = fs_ioctl(tf->ebx, tf->ecx, (void *)tf->edx);
}

static void sys_write(TrapFrame *tf) {
	int fd = tf->ebx;
	char* buf = (char*)tf->ecx;
	int len = tf->edx;
	if (fd == 1 || fd == 2) {
		//asm volatile (".byte 0xd6" : : "a"(2), "c"(buf), "d"(len));
		int i = 0;
		for (i = 0; i < len; i++) {
			serial_printc(buf[i]);
		}
		tf->eax = len;
	}
	else
	{
		tf->eax = fs_write(fd, buf, len);
	}
}

static void sys_open(TrapFrame *tf) {
	const char *pathname = (const char *)tf->ebx;
	int flags = tf->ecx;
	tf->eax = fs_open(pathname, flags);
}

static void sys_close(TrapFrame *tf) {
	int fd = tf->ebx;
	tf->eax = fs_close(fd);
}

static void sys_read(TrapFrame *tf) {
	int fd = tf->ebx;
	char *buf = (char *) tf->ecx;
	size_t len = tf->edx;
	tf->eax = fs_read(fd, buf, len);
}

static void sys_lseek(TrapFrame *tf) {
	int fd = tf->ebx;
	off_t offset = tf->ecx;
	int whence = tf->edx;
	tf->eax = fs_lseek(fd, offset, whence);
}


void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The `add_irq_handle' system call is artificial. We use it to
		 * let user program register its interrupt handlers. But this is
		 * very dangerous in a real operating system. Therefore such a
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;
		case SYS_ioctl: sys_ioctl(tf); break;

		/* TODO: Add more system calls. */

		case SYS_write: sys_write(tf); break;
		case SYS_read: sys_read(tf); break;
		case SYS_open: sys_open(tf); break;
		case SYS_lseek: sys_lseek(tf); break;
		case SYS_close: sys_close(tf); break;

		default: panic("Unhandled system call: id = %d, eip = 0x%08x", tf->eax, tf->eip);
	}
}

