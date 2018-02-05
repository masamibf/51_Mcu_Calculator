#include "Calculate.h"


uchar oprt = 0;                         //运算符类型
uchar step = 0;                         //操作步骤变量

uchar result_buf[12];                   //保存运算结果的缓冲区
uchar result_len;                       //运算结果长度

float num1 = 0;                         //运算数1
float num2 = 0;                         //运算数2
float result = 0;                       //运算结果

uchar num1_buf[10];                     //保存第一个运算数的缓冲区
uchar num2_buf[10];                     //保存第二个运算数的缓冲区
uchar num1_index = 0;                   //运算数一 索引变量
uchar num2_index = 0;                   //运算数二 索引变量

bit dec_flag = 0;                       //小数点标志

/*========================================================================*/
/*函数名称:void NumAction(uchar n)
/*函数功能:数字输入
/*输入参数:n
/*输出参数:无
/*========================================================================*/
void NumAction(uchar n)
{
	if(step>1)Reset();                      //完成运算则复位
	
	if(step == 0)                           //输入第一个运算数
	{
		if(num1_index < 10)                 //运算数长度不超过 10 位
		{
			num1_buf[num1_index] = n + '0';
			num1_index ++;
			LcdShowNum(1,1);                //在LCD1602第二行显示运算数一
		}
		else
			return;
	}
	else if(step == 1)                      //输入第二个运算数
	{
		if(num2_index < 10)
		{
			num2_buf[num2_index] = n + '0';
			num2_index ++;
			LcdShowNum(1,2);                //在LCD1602第二行显示运算数二
		}
		else
			return;
	}
}

/*========================================================================*/
/*函数名称:void DecimalAction()
/*函数功能:小数点输入
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void DecimalAction()
{	
	if(step == 0)                             //在第一个运算数中加入小数点
	{
		if((num1_index < 10)&&(dec_flag == 0))
		{
			dec_flag = 1;                     //每个运算数只能一个小数点
			num1_buf[num1_index] = '.';
			num1_index ++;
			LcdShowNum(1,1);
		}
		else
			return;
	}
	
	else if(step == 1)                        //在第二个运算数中加入小数点
	{
		if((num2_index < 10)&&(dec_flag == 0))
		{
			dec_flag = 1;
			num2_buf[num2_index] = '.';
			num2_index ++;
			LcdShowNum(1,2);
		}
		else
			return;
	}
}

/*========================================================================*/
/*函数名称:void OprAction(uchar type)
/*函数功能:运算符输入
/*输入参数:type
/*输出参数:无
/*========================================================================*/
void OprAction(uchar type)
{
	if(step==0)                              //第二个运算数未输入时响应
	{
		num1 = StringToFloat(num1_buf);      //将运算数缓冲区的字符转化为浮点数
		dec_flag = 0;                        //标志清零
		LcdAreaClear(0,0,16-num1_index);     //清除第一行左边的字符
		LcdShowNum(0,1);                     //字符显示在第一行右边
		LcdShowOpr(0,1,type);                //运算符显示在第二行左边
		LcdAreaClear(1,1,14);                //清除第二行中间的字符
		LcdShowStr(15,1,"0");
		oprt = type;                         //记录当前运算符
		num1_index = 0;
		step = 1;
	}
	
	if(step == 2)                               //连续输入状态
	{
		dec_flag = 0;
		num1 = result;                          //运算数一 为上一次运算结果
		LcdAreaClear(0,0,16);
		LcdShowStr(16-result_len,0,result_buf);	//上一次运算结果显示在第一行
		LcdAreaClear(0,1,16);
		LcdShowOpr(0,1,type);                   //运算符显示在第二行左边
		oprt = type;
		step = 1;
	}
}

/*========================================================================*/
/*函数名称:void GetResult()
/*函数功能:计算结果
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void GetResult()
{
	num2 = StringToFloat(num2_buf);
	
	if(step==1)                                        //第二运算数输入后才计算
	{
		step=2;
		dec_flag = 0;                                  //小数点标志清零
		switch(oprt)
		{
			case 0:result=(float)(num1+num2);break;
			case 1:result=(float)(num1-num2);break;
			case 2:result=(float)(num1*num2);break;
			case 3:result=(float)(num1/num2);break;
			case 4:result=(float)pow(num1,num2);break;
			default:break;
		}
		LcdShowOpr(0,0,oprt);                               //第一行显示运算符
		LcdAreaClear(1,0,16-num2_index);                    //清除运算数二的显示     
		LcdShowNum(0,2);                                    //运算数二显示到第一行
		LcdShowStr(0,1,"=");                                //第二行显示 等于号 =
		num2_index = 0;
		
		if((result < 99999999)&&(result > -99999999))       //运算结果在范围内
		{
			if(result == (signed long)result)                 //如果结果是整型
			{
				result_len = LongToString(result_buf,result); //转为字符串
				Save_Result();					              //保存结果
				LcdAreaClear(1,1,16);
				LcdShowStr(16-result_len,1,result_buf);		  //显示结果
			}
			else                                              //如果结果不为整型
			{ 
				if((result >=0)&&(result < 0.0001))result = 0;
				if((result <=1)&&(result > 0.9999))result = 1;
				result_len = FloatToString(result_buf,result);
				Save_Result();
				LcdAreaClear(1,1,16);
				LcdShowStr(16-result_len,1,result_buf);			
			}
		}
		else                                                  //运算结果超过范围则警告
			Warning();
	}
	memset(num1_buf,0,sizeof(num1_buf));                      //每次计算完成后将缓冲区清零
	memset(num2_buf,0,sizeof(num2_buf));
}

/*========================================================================*/
/*函数名称:void Save_Result()
/*函数功能:保存运算结果
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void Save_Result()
{
	E2P_Write(0x00,0x40,20);                     //首先清除上一个结果
	E2P_Write(result_buf,0x40,result_len);       //将本次运算结果存入24c02
}

/*========================================================================*/
/*函数名称:void Read_Result()
/*函数功能:读取运算结果
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void Read_Result()
{
	uchar j;
	uchar tmp[12];
	
	LcdClearAll();
	E2P_Read(tmp,0x40,sizeof(result_buf));      //从24c02读取运算结果
	
	for(j = 0;j < result_len;j ++)
	{
			LcdShowDat(j,0,tmp[j]);             //显示读取的结果
	}
}

/*========================================================================*/
/*函数名称:void LcdShowOpr(uchar x,uchar y,uchar type)
/*函数功能:lcd显示操作符
/*输入参数:坐标(x,y) 操作符类型type
/*输出参数:无
/*========================================================================*/
void LcdShowOpr(uchar x,uchar y,uchar type)
{
	switch(type)
	{
		case 0:LcdShowStr(x,y,"+");break;         // 加号
		case 1:LcdShowStr(x,y,"-");break;         // 减号
		case 2:LcdShowDat(x,y,0x78);break;        // 乘号
		case 3:LcdShowDat(x,y,0xFD);break;        // 除号
		case 4:LcdShowStr(x,y,"^");break;         // 乘方
		default:break;
	}
}

/*========================================================================*/
/*函数名称:void LcdShowNum(uchar y,uchar type)
/*函数功能:lcd1602显示运算数
/*输入参数:行数y ,运算数类型type
/*输出参数:无
/*========================================================================*/
void LcdShowNum(uchar y,uchar type)
{
	if(y == 0)                                       //在LCD1602第一行显示
	{
		if(type == 1)
			LcdShowStr(16-num1_index,0,num1_buf);    //显示运算数一
		else if(type == 2)
			LcdShowStr(16-num2_index,0,num2_buf);    //显示运算数二
	}
	
	else if(y == 1)                                  //在LCD1602第二行显示
	{
		if(type == 1)
			LcdShowStr(16-num1_index,1,num1_buf);
		else if(type == 2)
			LcdShowStr(16-num2_index,1,num2_buf);
	}
}

/*========================================================================*/
/*函数名称:void Reset()
/*函数功能:计算器复位
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void Reset()
{
	num1 = 0;                                //将运算数 标志位 索引 缓冲区 清零
	num2 = 0;
	dec_flag = 0;
	memset(num1_buf,0,sizeof(num1_buf));
	memset(num2_buf,0,sizeof(num2_buf));
	num1_index = 0;
	num2_index = 0;
	step = 0;
	LcdClearAll();
	LcdShowStr(15,1,"0");
}

/*========================================================================*/
/*函数名称:void Warning()
/*函数功能:提出警告
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void Warning()
{
	uint i = 10000;
	
	Reset();
	
	if(Buzz_flag == 1)
	{
		while(i--)
		{
			Buzz = ~Buzz;
		}		
	}
	
	LcdShowStr(0,0,"    ERROR!");
	Delay100ms();
	Reset();
	LcdShowStr(15,1,"0");
}