#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <reg52.h>
#include "intrins.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
	
sbit Buzz = P2^4;

extern bit Buzz_flag;

void BuzzerControl();
void Delay100us();
void Delay100ms();

#endif