/*=================================================*/
/*项目名称: 51单片机计算器
/*代码作者: F H X
/*修改时间: 2017/12/20
/*联系方式: 374454765@qq.com
/*=================================================*/
#include "Config.h"

uchar T0RH = 0;                                  //定时器0重载值
uchar T0RL = 0;

extern bit Buzz_flag;                            //蜂鸣器开关标志
/*========================================================================*/
/*函数名称:void main()
/*函数功能:主函数
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void main()
{
	EA=1;                                          //开启总中断
	Timer0Init(2);                                 //定时器0初始化 定时2ms
	InitLcd1602();                                 //LCD1602初始化
	
	while(1)
	{	
		KeyDrive();                                  //矩阵键盘驱动
		KeyDrive2();                                 //独立键盘驱动
	}
}
/*========================================================================*/
/*函数名称:void KeyDown(uchar keycode)
/*函数功能:执行按键动作
/*输入参数:keycode
/*输出参数:无
/*========================================================================*/
void KeyDown(uchar keycode)
{
	if((keycode <= 0x39)&&(keycode >= 0x30))           //数字键0~9
	{
		BuzzerControl();
		NumAction(keycode - 0x30);
	}
	else if(keycode == 0x26)                           //"+"号
	{
		BuzzerControl();
		OprAction(0);
	}
	else if(keycode == 0x25)                           //"-"号
	{
		BuzzerControl();
		OprAction(1);
	}
	else if(keycode == 0x28)                           //"×"号
	{
		BuzzerControl();
		OprAction(2);
	}
	else if(keycode == 0x27)                           //"÷"号
	{
		BuzzerControl();
		OprAction(3);
	}
	else if(keycode == 0x0D)                           //"="号
	{
		BuzzerControl();
		GetResult();
	}
	else if(keycode == 0x1B)                           //小数点 '.'
	{
		BuzzerControl();
		DecimalAction();
	}
	else if(keycode == 0x01)                           //复位键
	{
		BuzzerControl();
		Reset();
	}
	else if(keycode == 0x02)                           //乘方运算                   
	{
		BuzzerControl();
		OprAction(4);	
	}
	else if(keycode == 0x03)                           //从24c02读取运算结果
	{
		BuzzerControl();
		Read_Result();                               
	}
	else if(keycode == 0x04)                           //开关蜂鸣器声音
	{
		Buzz_flag = ~Buzz_flag;
	}
}
/*========================================================================*/
/*函数名称:void Timer0Init(uint ms)
/*函数功能:定时器0初始化
/*输入参数:ms
/*输出参数:无
/*========================================================================*/
void Timer0Init(uint ms)
{
	ulong tmp;                                //16位长整型变量tmp
	tmp = MAIN_OSC/12;                        //用晶振频率除以12得到定时器计数频率
	tmp = (tmp*ms)/1000;                      //计算出需要多少个机器周期
	tmp = 65536 - tmp;                        //定时器的初始值
	tmp = tmp + 18;                           //补偿中断延时响应造成的误差
	T0RH = (uchar)(tmp>>8);                   //将tmp高8位右移8位 赋值给T0RH
	T0RL = (uchar)tmp;                        //将tmp低8位赋值给T0RL
	TMOD &= 0xF0;                             //TMOD高4位不变 低四位清零
	TMOD |= 0x01;                             //TMOD低四位变成0001
	TH0 = T0RH;                               //装载初值
	TL0 = T0RL;
	ET0 = 1;                                  //开启定时器0中断          
	TR0 = 1;                                  //开启定时器0
}
/*========================================================================*/
/*函数名称:void InterruptTimer0() interrupt 1
/*函数功能:定时器0中断服务
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void InterruptTimer0() interrupt 1
{
	TH0 = T0RH;                               //重载初值
	TL0 = T0RL;
	KeyScan();                               //矩阵按键扫描
	KeyScan2();                              //独立按键扫描
}