#ifndef __KEY_H__
#define __KEY_H__

#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
	
sbit KEY_IN_1 = P1^3;
sbit KEY_IN_2 = P1^2;
sbit KEY_IN_3 = P1^1;
sbit KEY_IN_4 = P1^0;
sbit KEY_OUT_1 = P1^4;
sbit KEY_OUT_2 = P1^5;
sbit KEY_OUT_3 = P1^6;
sbit KEY_OUT_4 = P1^7;

void KeyDrive();
void KeyScan();

extern void KeyDown(uchar keycode);

#endif