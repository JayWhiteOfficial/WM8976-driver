#ifndef I2C_H_
#define I2C_H_

#include <s5pv210-serial-stdio.h>


void i2c_init(void);
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len);
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len);
void do_irq(void);
void Delay(int time);
unsigned char at24cxx_read(unsigned char address);
void at24cxx_write(unsigned char address, unsigned char data);
void wm8960_write(unsigned int slave_addr, int addr, int data);


#endif
