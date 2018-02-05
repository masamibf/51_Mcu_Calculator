/*========================*/
/*  包含字符串处理函数    */
/*========================*/
#include "StrPro.h"

/*========================================================================*/
/*函数名称:uchar LongToString(uchar *str,float dat)
/*函数功能:长整型转为字符串
/*输入参数:字符串*str 长整型数dat
/*输出参数:字符串长度len
/*========================================================================*/
uchar LongToString(uchar *str,float dat)
{
	uchar len;
	
	sprintf(str,"%10ld",(long int)dat);
	len = sprintf(str,"%10ld",(long int)dat);
	
	return len;     	                           //返回len
}

/*========================================================================*/
/*函数名称:uchar FloatToString(uchar *str,float dat)
/*函数功能:浮点型转为字符串
/*输入参数:字符串*str 浮点型数dat
/*输出参数:字符串长度len
/*========================================================================*/
uchar FloatToString(uchar *str,float dat)
{
	uchar len;
	
	sprintf(str,"%5.5f",dat);
	
	len = sprintf(str,"%5.5f",dat);
	
	while(len-- > 0)                              //去除末尾多余的 0
	{
		if(str[len] != '0')
		{
			break;
		}
	}
	
	len = len + 1;
	return len;
}

/*========================================================================*/
/*函数名称:float StringToFloat(uchar *buf)
/*函数功能:字符转为浮点数
/*输入参数:字符数组 buf
/*输出参数:浮点数 num
/*========================================================================*/
float StringToFloat(uchar *buf)
{	
	float num;
	
	num = atof(buf);
	
	return num;
}