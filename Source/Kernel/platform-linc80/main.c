#include <kernel.h>
#include <kdata.h>
#include <printf.h>
#include <timer.h>
#include <devtty.h>

extern unsigned char irqvector;
struct blkbuf *bufpool_end = bufpool + NBUFS;	/* minimal for boot -- expanded after we're done with _DISCARD */
uint16_t swap_dev = 0xFFFF;
uint16_t ramtop = 0xFFFF;

void platform_discard(void)
{
	while (bufpool_end < (struct blkbuf *) (KERNTOP - sizeof(struct blkbuf))) {
		memset(bufpool_end, 0, sizeof(struct blkbuf));
#if BF_FREE != 0
		bufpool_end->bf_busy = BF_FREE;	/* redundant when BF_FREE == 0 */
#endif
		bufpool_end->bf_dev = NO_DEVICE;
		bufpool_end++;
	}
	kprintf("Buffers available: %d\n", bufpool_end - bufpool);
}

static uint8_t idlect;

void platform_idle(void)
{
	__asm
		halt
	__endasm;
}

uint8_t platform_param(unsigned char *p)
{
	used(p);
	return 0;
}

void platform_interrupt(void)
{
	tty_drain_sio();
	timer_interrupt();
}

