#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#include<reg52.h>

#include "24c02.h"
#include "StrPro.h"
#include "math.h"
#include "1602.h"
#include "Buzzer.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

void LcdShowOpr(uchar x,uchar y,uchar type);
void NumAction(uchar n);
void DecimalAction();
void OprAction(uchar type);
void GetResult();
void LcdShowNum(uchar y,uchar type);
void Reset();
void Warning();
void Save_Result();
void Read_Result();

#endif