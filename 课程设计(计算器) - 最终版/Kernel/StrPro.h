#ifndef __STRPRO_H__
#define __STRPRO_H__

#include<reg52.h>

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

uchar LongToString(uchar *str,float dat);
uchar FloatToString(uchar *str,float dat);
float StringToFloat(uchar *buf);


#endif