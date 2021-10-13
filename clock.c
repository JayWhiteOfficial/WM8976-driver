#include <clock.h>
#define APLL_CON      (*(volatile unsigned int *)0xe0100100)
#define CLK_SRC0      (*(volatile unsigned int *)0xe0100200)
#define CLK_DIV0      (*(volatile unsigned int *)0xe0100300)
#define MPLL_CON      (*(volatile unsigned int *)0xe0100108)

void clock_init(void)
{
    // set clock as:
    // ARMCLK=1000MHz, HCLKM=200MHz, HCLKD=166.75MHz
    // HCLKP =133.44MHz, PCLKM=100MHz, PCLKD=83.375MHz,
    // PCLKP =66.7MHz
	//FoutAPLL = 667MHz
	APLL_CON = (1<<31)|(1<<29)|(0x7d<<16)|(0x3<<8)|(1<<0);
    CLK_SRC0 = (1<<28)|(1<<12)|(1<<8)|(1<<4)|(1<<0);
    CLK_DIV0 = (0x1<<28)|(0x4<<24)|(0x1<<20)|(0x3<<16)|(0x1<<12)|(0x4<<8)|(0x4<<4);
    APLL_CON = (1<<31)|(1<<29)|(0x29d<<16)|(0xc<<8)|(1<<0);
}
