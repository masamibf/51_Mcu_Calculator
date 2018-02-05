//**********************Disigned By F H X***************************//
//*********************4位独立按键扫描程序**************************//
//*****1 按键驱动函数 ：检测按键动作 执行 mian.c 中按键动作函数*****//
//*****2 按键扫描函数 ：扫描按键状态 在main.c 中的中断函数被调用****//
#include "key2.h"


uchar code KeyCodeMap2[4] = {0x01,0x02,0x03,0x04};         // 
uchar KeySta2[4] = {1,1,1,1};                              // 按键当前状态 默认为弹起
ulong pdata KeyDownTime2[4] = {0,0,0,0};                   // 按键按下时间 默认为0

extern void KeyDown(uchar keycode);

/*========================================================================*/
/*函数名称:void KeyDrive2()
/*函数功能:独立按键驱动
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void KeyDrive2()
{
	uchar i;
	static uchar backup2[4] = {1,1,1,1};                     //保存上次按键状态 默认都为弹起
	static ulong pdata TimeTh2[4] = {1000,1000,1000,1000};   //按键按下时间阈值 超过阈值则为长按模式
	for(i=0;i<4;i++)
	{
		if(backup2[i]!=KeySta2[i])                              //检测按键动作
		{
			if(KeySta2[i]==0)                                    //按键被按下	
			{
				KeyDown(KeyCodeMap2[i]);                           //执行按键动作函数
			}
			backup2[i]=KeySta2[i];                                //保存当前按键状态
		}
    if(KeyDownTime2[i]>0)                                  //检测按键按下时间
		{
			if(KeyDownTime2[i]>=TimeTh2[i])                       //按键按下时间大于阈值 执行按键动作函数
			{
				KeyDown(KeyCodeMap2[i]);
				TimeTh2[i]+=200;                                   //阈值自增
			}
		}
		else 
		{
			TimeTh2[i]=1000;                                     //按键弹起时恢复阈值时间
		}
	}
}

/*========================================================================*/
/*函数名称:void KeyScan2()
/*函数功能:独立按键扫描
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void KeyScan2()
{
	uchar i;
	static uchar Keybuff2[4]={0xFF,0xFF,0xFF,0xFF};          //按键扫描缓冲区
	
	Keybuff2[0]=(Keybuff2[0]<<1)|KEY_1;                       //逐个扫描4个独立按键 起到延时消抖作用
	Keybuff2[1]=(Keybuff2[1]<<1)|KEY_2;
	Keybuff2[2]=(Keybuff2[2]<<1)|KEY_3;
	Keybuff2[3]=(Keybuff2[3]<<1)|KEY_4;
	for(i=0;i<4;i++)
	{
		if((Keybuff2[i]&0x0F)==0x00)                           //检测到某个按键被按下
		{
			KeySta2[i]=0;                                        //改变该按键状态
			KeyDownTime2[i]+=4;                                  //该按键按下时间累加
		}
		else if((Keybuff2[i]&0x0F)==0x0F)                      //检测到某个按键弹起
		{
			KeySta2[i] = 1;                                      //改变该按键状态
      KeyDownTime2[i] = 0;                                 //该按键按下时间清零
		}
  }
}