#ifndef __24C02_H__
#define __24C02_H__

#include "reg52.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
	
#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}


sbit I2C_SCL = P2^3;
sbit I2C_SDA = P2^2;

void E2P_Read(uchar *buf,uchar addr,uchar len);
void E2P_Write(uchar *buf,uchar addr,uchar len);

#endif