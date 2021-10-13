#include "key.h"
#include <types.h>
#include <io.h>
#include <s5pv210/reg-gpio.h>
void key_init(void)
{
	/*
	 * LEFT ->EINT2   -> GPH0_2
	 * DOWN ->EINT3   -> GPH0_3
	 * UP   ->KP_COL0 -> GPH2_0
	 * RIGHT->KP_COL1 -> GPH2_1
	 * BACK ->KP_COL2 -> GPH2_2
	 * MENU ->KP_COL3 -> GPH2_3
	 * */
	/* LEFT */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH0CON) & ~(0xf<<8)) | (0x0<<8));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH0PUD) & ~(0x3<<4)) | (0x2<<4));
	/* DOWN */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH0CON) & ~(0xf<<12)) | (0x0<<12));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH0PUD) & ~(0x3<<6)) | (0x2<<6));
	/* UP */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<0)) | (0x0<<0));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<0)) | (0x2<<0));
	/* RIGHT */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<4))| (0x0<<4));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<2)) | (0x2<<2));
	/* BACK */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<8))| (0x0<<8));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<4)) | (0x2<<4));
	/* MENU */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<12))| (0x0<<12));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<6)) | (0x2<<6));
}
