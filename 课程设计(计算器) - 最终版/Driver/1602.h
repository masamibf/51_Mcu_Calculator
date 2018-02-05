#ifndef  __1602_H__
#define  __1602_H__

#include<reg52.h>
#define LCD1602_DB P0
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
	
sbit LCD1602_RS = P2^7;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E = P2^5;

void LcdWriteCmd(uchar cmd);
void InitLcd1602();
void LcdSetCursor(uchar x,uchar y);
void LcdWriteDat(uchar dat);
void LcdShowStr(uchar x,uchar y,uchar *str);
void LcdAreaClear(uchar x,uchar y,uchar len);
void LcdClearAll();
void LcdShowDat(uchar x,uchar y,uchar num);

#endif