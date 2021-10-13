#include <i2s.h>
#define GPICON      (*(volatile unsigned int *)0xE0200220)    //IIS Signals

#define IISCON      (*(volatile unsigned int *)0xEEE30000)    //IIS Control
#define IISMOD      (*(volatile unsigned int *)0xEEE30004)    //IIS Mode
#define IISFIC      (*(volatile unsigned int *)0xEEE30008)    //IIS FIFO Control
#define IISPSR      (*(volatile unsigned int *)0xEEE3000C)    //IIS Prescaler
#define IISTXD      (*(volatile unsigned int *)0xEEE30010)    //IIS TXD DATA
#define IISRXD      (*(volatile unsigned int *)0xEEE30014)    //IIS RXD DATA
#define IISFICS     (*(volatile unsigned int *)0xEEE30018)    //IIS FIFO Control
//CLCK
#define EPLL_CON0   (*(volatile unsigned int *)0xe0100110)
#define EPLL_CON1   (*(volatile unsigned int *)0xe0100114)
#define CLK_SRC0    (*(volatile unsigned int *)0xE0100200)
#define CLK_CON     (*(volatile unsigned int *)0xEEE10000)

void IIS_init(void)

{
    GPICON = 0x22222222;
    EPLL_CON0 = 0xa8430303;    // MPLL_FOUT = 67.7Mhz
    EPLL_CON1 = 0xbcee;          // K = 0xbcee
    CLK_SRC0 = 0x10001111;
    CLK_CON = 0x1;
    IISPSR = 1<<15 | 5<<8;
    IISCON |= 1<<0 | (unsigned)1<<31;
    IISMOD = 1<<9 | 0<<8 | 1<<10;
}
