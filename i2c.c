#include <i2c.h>
#define GPD1CON        (*(volatile unsigned int *)0xE02000C0)
#define GPD1PUD        (*(volatile unsigned int *)0xE02000C8)
// IIC
#define IICCON         (*(volatile unsigned int *)0xE1A00000)
#define IICSTAT        (*(volatile unsigned int *)0xE1A00004)
#define IICDS          (*(volatile unsigned int *)0xE1A0000C)
#define VIC0ADDRESS    (*(volatile unsigned int *)0xF2000F00)
#define VIC1ADDRESS    (*(volatile unsigned int *)0xF2100F00)
#define VIC2ADDRESS    (*(volatile unsigned int *)0xF2200F00)
#define VIC3ADDRESS    (*(volatile unsigned int *)0xF2300F00)

void Delay(int time);
#define WRDATA      (1)
#define RDDATA      (2)

typedef struct tI2C {
    unsigned char *pData;   // data buffer
    volatile int DataCount; // data length
    volatile int Status;    // status
    volatile int Mode;      // mode:write or read
    volatile int Pt;        // location of data to be transmitted
}t210_I2C, *pt210_I2C;

static t210_I2C g_t210_I2C;

void i2c_init(void)
{
    GPD1CON |= 0x220000;
    GPD1PUD |= 0x500;
    IICCON = (1<<7) | (0<<6) | (1<<5) | (0xf);
    IICSTAT = 0x10;
}

void i2c_write(unsigned int slvAddr, unsigned char *buf, int len)
{
    g_t210_I2C.Mode = WRDATA;
    g_t210_I2C.Pt   = 0;
    g_t210_I2C.pData = buf;
    g_t210_I2C.DataCount = len;
    IICDS   = slvAddr;
    IICSTAT = 0xf0;
    while (g_t210_I2C.DataCount != -1); //wait till transmitting is over
}

void i2c_read(unsigned int slvAddr, unsigned char *buf, int len)

{
    g_t210_I2C.Mode = RDDATA;
    g_t210_I2C.Pt   = -1;
    g_t210_I2C.pData = buf;
    g_t210_I2C.DataCount = len;
    while (g_t210_I2C.DataCount != 0);
}

//----------IIC IRQ----------

void do_irq(void)
{
    unsigned int iicSt,i;
    iicSt  = IICSTAT;
    switch (g_t210_I2C.Mode)
    {
        case WRDATA:
        {
            if((g_t210_I2C.DataCount--) == 0)
            {
                IICSTAT = 0xd0;
                IICCON  = 0xaf;
                Delay(10000);  // wait till signal P is transmitted
                break;
            }
            IICDS = g_t210_I2C.pData[g_t210_I2C.Pt++];
            for (i = 0; i < 10; i++);
            IICCON = 0xaf;
            break;
        }
        case RDDATA:
        {
            if (g_t210_I2C.Pt == -1)
            {
                //this interruption occurs after transmitting the servers location, thus no data
                //do not transmit ACK when you receive only one signal
                g_t210_I2C.Pt = 0;
                if(g_t210_I2C.DataCount == 1)
                    IICCON = 0x2f;
                else
                    IICCON = 0xaf;
                break;
            }
            g_t210_I2C.pData[g_t210_I2C.Pt++] = IICDS;
            g_t210_I2C.DataCount--;
            if (g_t210_I2C.DataCount == 0)
            {
                IICSTAT = 0x90;
                IICCON  = 0xaf;
                Delay(10000);  // wait till signal P is transmitted
                break;
            }
            else
            {
                //do not transmit ACK when it is the last data
                if(g_t210_I2C.DataCount == 1)
                    IICCON = 0x2f;
                else

                    IICCON = 0xaf;
            }
            break;
        }
        default:
            break;
    }
    // clear VICADDRESS
    VIC0ADDRESS = 0x0;
    VIC1ADDRESS = 0x0;
    VIC2ADDRESS = 0x0;
    VIC3ADDRESS = 0x0;
}

void Delay(int time)

{
    for (; time > 0; time--);
}

void wm8976_write(unsigned int slave_addr, int addr, int data)
{
    unsigned char val[2];
    val[0] = addr<<1 | ((data>>8) & 0x0001);
    val[1] = (data & 0x00FF);
    i2c_write(slave_addr, val, 2);
    mdelay(10);//wait for a while to ensure the i2c_write has been finished
}
