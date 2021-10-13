#ifndef __KEY_H__
#define __KEY_H__

#define S5PV210_GPH2CON	 	 	(0xE0200000 + 0xC40)
#define S5PV210_GPH2DAT	 	 	(0xE0200000 + 0xC44)
#define S5PV210_GPH2PUD	 	 	(0xE0200000 + 0xC48)
#define S5PV210_GPH2DRV	 	 	(0xE0200000 + 0xC4C)
#define S5PV210_GPH0CON	 	 	(0xE0200000 + 0xC00)
#define S5PV210_GPH0DAT	 	 	(0xE0200000 + 0xC04)
#define S5PV210_GPH0PUD	 	 	(0xE0200000 + 0xC08)
#define S5PV210_GPH0DRV	 	 	(0xE0200000 + 0xC0C)
void key_init(void);

#endif




