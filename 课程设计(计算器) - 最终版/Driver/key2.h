#ifndef __KEY2_H__
#define __KEY2_H__

#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
	
sbit KEY_1 = P3^4;
sbit KEY_2 = P3^5;
sbit KEY_3 = P3^6;
sbit KEY_4 = P3^7;

void KeyDrive2();
void KeyScan2();

extern void KeyDown(uchar keycode);

#endif
