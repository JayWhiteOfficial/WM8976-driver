#include <io.h>
#include "clock.h"
#include"caihong.h"
#include <s5pv210-serial-stdio.h>
#include "key.h"
#include "i2c.h"
#include "i2s.h"
#include "readline.h"



#define IISCON        (*(volatile unsigned int *)0xEEE30000)    //IIS Control
#define IISTXD        (*(volatile unsigned int *)0xEEE30010)    //IIS TXD DATA
#define MAX_MUSIC_SIZE 533610-1									//max size of music array
#define wm8976_DEVICE_ADDR        0x34
#define REFINE_RATE 5000*3										//the speed to refine volume and frequency


extern unsigned int caihong[]; // music array


void wm8976_init(void)
{
    wm8976_write(wm8976_DEVICE_ADDR,0x0,0x0);                                   //reset
    wm8976_write(wm8976_DEVICE_ADDR,0x1,0x1f);
    wm8976_write(wm8976_DEVICE_ADDR,0x2,0x185);                                 //output enable
    wm8976_write(wm8976_DEVICE_ADDR,0x3,0x6f);                                  //channel mixer enable
    wm8976_write(wm8976_DEVICE_ADDR,0x4,0x10);                                  //set IIS format
    wm8976_write(wm8976_DEVICE_ADDR,0x6,0x0);                                   //set MCLK as source clock
    wm8976_write(wm8976_DEVICE_ADDR,0x7,0x1);                                   //enable slow clock
    wm8976_write(wm8976_DEVICE_ADDR,0xd,0x21);                                  //set Jack Detection
    wm8976_write(wm8976_DEVICE_ADDR,0x2b,0x10);
    //set mixer channels and volume
    wm8976_write(wm8976_DEVICE_ADDR,0x32,0x1);                                  //set left DAC as input of left channel
    wm8976_write(wm8976_DEVICE_ADDR,0x33,0x1);                                  //set right DAC as input of right channel
    wm8976_write(wm8976_DEVICE_ADDR,0x34,0x139);								//set left headphone volume
    wm8976_write(wm8976_DEVICE_ADDR,0x35,0x139);								//set right headphone volume
    return;
}

void message(void)
{
    serial_printf(2, "\nThis program supports music playing:\n");
    serial_printf(2, "Please set your music as a head file and add it to the program\n");
}

void play_music()
{
	int i = 0;
	unsigned int step = 1;
	int time = 0;
	int volume = 0;
	serial_printf(2, "loading wav...\n");
	while(1)
	{
		while((IISCON & (1<<8)) == (1<<8)){};   //wait till FIFO is not empty
		IISTXD = caihong[i];
		i+=step;
		if(i >= MAX_MUSIC_SIZE)                 //reset i when i reaches MAX_MUSIC_SIZE
		{
			i = 0;
		}
		if(!(readl(S5PV210_GPH0DAT) & (0x1<<2)))	//GPH0_2£¬LEFT, volume down
		{
			if(volume>-56)
			{
				time++;
				if(time == REFINE_RATE)
				{
					time = 0;
					volume--;
					wm8976_write(wm8976_DEVICE_ADDR,0x35,0x139+volume);
					wm8976_write(wm8976_DEVICE_ADDR,0x34,0x139+volume);
					serial_printf(2, "current volume: %d dB\n", volume);
				}
			}
		}
		if(!(readl(S5PV210_GPH0DAT) & (0x1<<3)))	//GPH0_3£¬DOWN, volume up
		{

			if(volume<6)
			{
				time++;
				if(time == REFINE_RATE)
				{
					time = 0;
					volume++;
					wm8976_write(wm8976_DEVICE_ADDR,0x35,0x139+volume);
					wm8976_write(wm8976_DEVICE_ADDR,0x34,0x139+volume);
					serial_printf(2, "current volume: %d dB\n", volume);
				}
			}
		}
		if(!(readl(S5PV210_GPH2DAT) & (0x1<<1)))	//GPH2_1£¬RIGHT, frequency down
		{
			if(step > 1)
			{
				time++;
				if(time == REFINE_RATE)
				{
					time = 0;
					step--;
				}
			}
		}
		if(!(readl(S5PV210_GPH2DAT) & (0x1<<2)))	//GPH2_2£¬BACK   frequency up
		{
			if(step < 10)
			{
				time++;
				if(time == REFINE_RATE)
				{
					time = 0;
					step++;
				}
			}
		}
	}
}

int wm8976_play(void)
{
    clock_init();
    i2c_init();
    wm8976_init();
    IIS_init();
    message();
    play_music();
    return 0;
}
