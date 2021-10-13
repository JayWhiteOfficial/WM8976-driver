#include <main.h>

extern int wm8976(int argc, char * argv[]);

static void do_system_initial(void)
{
	malloc_init();
	s5pv210_clk_initial();
	s5pv210_irq_initial();
	s5pv210_tick_initial();
	s5pv210_tick_delay_initial();
	s5pv210_serial_initial();
	s5pv210_fb_initial();
	irq_init();
	key_initial();
}

int main(int argc, char * argv[])
{
	do_system_initial();
	serial_printf(2, "system successfully initial\n");
	wm8976_play();
	return 0;
}
