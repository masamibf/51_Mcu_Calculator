#include "1602.h"
	
/*========================================================================*/
/*函数名称:void LcdWriteReady()
/*函数功能:检测1602是否忙
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void LcdWriteReady()
{
	uchar sta;                                
	LCD1602_DB=0xFF;                          
	LCD1602_RS=0;                             
	LCD1602_RW=1;
	do                                         
  {
		LCD1602_E=1;
		sta=LCD1602_DB;
		LCD1602_E=0;
	}while(sta&0x80);                       
}

/*========================================================================*/
/*函数名称:void LcdWriteCmd(uchar cmd)
/*函数功能:向Lcd写入命令
/*输入参数:cmd
/*输出参数:无
/*========================================================================*/
void LcdWriteCmd(uchar cmd)
{
	LcdWriteReady();                           
	LCD1602_RS=0;                             
	LCD1602_RW=0;
	LCD1602_DB=cmd;
	LCD1602_E=1;
	LCD1602_E=0;
}

/*========================================================================*/
/*函数名称:void LcdWriteDat(uchar dat)
/*函数功能:向Lcd写入数据
/*输入参数:dat
/*输出参数:无
/*========================================================================*/
void LcdWriteDat(uchar dat)
{
	LcdWriteReady();                           
	LCD1602_RS=1;                             
	LCD1602_RW=0;
	LCD1602_DB=dat;
	LCD1602_E=1;
	LCD1602_E=0;
}

/*========================================================================*/
/*函数名称:void InitLcd1602()
/*函数功能:初始化Lcd1602
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void InitLcd1602()
{
	LcdWriteCmd(0x38);                         
	LcdWriteCmd(0x0c);                        
	LcdWriteCmd(0x06);                      
	LcdWriteCmd(0x01);  
  LcdShowStr(15,1,"0");	
}

/*========================================================================*/
/*函数名称:void LcdSetCursor(uchar x,uchar y)    
/*函数功能:LCD设置坐标
/*输入参数:横坐标x 纵坐标 y
/*输出参数:无
/*========================================================================*/
void LcdSetCursor(uchar x,uchar y)          
{
	uchar addr;
	if(y==0)
	{
		addr=0x00+x;                         
	}
	else
	{
		addr=0x40+x;                             
	}
	LcdWriteCmd(addr|0x80);                   
}

/*========================================================================*/
/*函数名称:void LcdShowStr(uchar x,uchar y,uchar *str)
/*函数功能:LCD1602显示字符串
/*输入参数:横坐标x 纵坐标 y 字符串 str
/*输出参数:无
/*========================================================================*/
void LcdShowStr(uchar x,uchar y,uchar *str)
{
	LcdSetCursor(x,y);                         //设置起始坐标
	while(*str!='\0')                          //当字符串没有结束时连续写入
	{
		LcdWriteDat(*str++);                     //先取str指向的数据 再将str+1
	}
}

/*========================================================================*/
/*函数名称:void LcdShowDat(uchar x,uchar y,uchar num)
/*函数功能:LCD1602显示数据
/*输入参数:横坐标x 纵坐标 y 数据 num
/*输出参数:无
/*========================================================================*/
void LcdShowDat(uchar x,uchar y,uchar num)
{
	LcdSetCursor(x,y);
	LcdWriteDat(num);
}

/*========================================================================*/
/*函数名称:void LcdAreaClear(uchar x,uchar y,uchar len)
/*函数功能:LCD1602区域清除
/*输入参数:横坐标x 纵坐标 y 长度 len
/*输出参数:无
/*========================================================================*/
void LcdAreaClear(uchar x,uchar y,uchar len)
{
	LcdSetCursor(x,y);
	while(len--)
	{
		LcdWriteDat(' ');
	}
}

/*========================================================================*/
/*函数名称:void LcdClearAll()
/*函数功能:LCD1602全部清除
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void LcdClearAll()
{
	LcdWriteCmd(0x01);
}

