#include "key.h"
//**********************Disigned By F H X***************************//
//*********************4*4矩阵键盘扫描程序**************************//
//*****1 按键驱动函数 ：检测按键动作 执行 main.c 中按键动作函数*****//
//*****2 按键扫描函数 ：扫描按键状态 在main.c 中的中断函数被调用****//


uchar code KeyCodeMap[4][4]={            
     {0x31,0x32,0x33,0x26},               //数字键1，数字键2，数字键3，向上键
     {0x34,0x35,0x36,0x25},               //数字键4，数字键5，数字键6，向左键
     {0x37,0x38,0x39,0x28},               //数字键7，数字键8，数字键9，向下键 
     {0x30,0x1B,0x0D,0x27}                //数字键0,  ESC键 ，回车键 ，向右键
};

uchar KeySta[4][4]={                      //16个按键当前状态
	   {1,1,1,1},
     {1,1,1,1},
     {1,1,1,1},
     {1,1,1,1} 
};                                        
ulong pdata KeyDownTime[4][4]={           //16个按键按下时间
     {0,0,0,0},
     {0,0,0,0},
     {0,0,0,0},
     {0,0,0,0} 
};  

/*========================================================================*/
/*函数名称:void KeyDrive()
/*函数功能:矩阵按键驱动
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void KeyDrive()
{
	uchar i,j;
	static uchar backup[4][4]={             //保存上次按键状态 默认都为弹起
     {1,1,1,1},
     {1,1,1,1},
     {1,1,1,1},
     {1,1,1,1}
	};
	static ulong pdata TimeTh[4][4]={       //按键按下时间阈值 超过阈值则为长按模式
     {1000,1000,1000,1000},
     {1000,1000,1000,1000},
     {1000,1000,1000,1000},
     {1000,1000,1000,1000}
	};
	for(i=0;i<4;i++)                        //循环扫描4×4矩阵键盘
	{
		for(j=0;j<4;j++)
		{
			if(backup[i][j]!=KeySta[i][j])      //检测按键动作
			{
				if(KeySta[i][j]==0)               //按键被按下
				{
					KeyDown(KeyCodeMap[i][j]);      //执行按键动作函数
				}
				backup[i][j]=KeySta[i][j];        //保存当前按键值
			}
			if(KeyDownTime[i][j]>0)             //检测按键按下时间
			{
				if(KeyDownTime[i][j]>=TimeTh[i][j])
				{
					KeyDown(KeyCodeMap[i][j]);      //按键按下时间大于阈值 执行按键动作函数
					TimeTh[i][j]+=200;              //阈值自增
				}
			}
			else 
			{
				TimeTh[i][j]=1000;                //按键弹起时恢复阈值时间
			}
		}
	}
}

/*========================================================================*/
/*函数名称:void KeyScan()
/*函数功能:矩阵按键扫描
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void KeyScan()
{
	uchar i;
	static uchar keyout=0;                  //矩阵按键扫描输出索引
	static uchar Keybuff[4][4]={            //按键扫描缓冲区
		{0xFF,0xFF,0xFF,0xFF},
    {0xFF,0xFF,0xFF,0xFF},
    {0xFF,0xFF,0xFF,0xFF},
    {0xFF,0xFF,0xFF,0xFF}
	};
	                                        //逐行检测按键是否被按下 起到按键消抖作用
  Keybuff[keyout][0]=(Keybuff[keyout][0]<<1)|KEY_IN_1;
	Keybuff[keyout][1]=(Keybuff[keyout][1]<<1)|KEY_IN_2;
	Keybuff[keyout][2]=(Keybuff[keyout][2]<<1)|KEY_IN_3;
	Keybuff[keyout][3]=(Keybuff[keyout][3]<<1)|KEY_IN_4;
	for(i=0;i<4;i++)
	{
		if((Keybuff[keyout][i]&0x0F)==0x00)     //检测某个按键被按下
		{
			KeySta[keyout][i]=0;                //改变该按键状态
			KeyDownTime[keyout][i]+=4;          //该按键按下时间累加
		}
		else if((Keybuff[keyout][i]&0x0F)==0x0F)//检测某个按键弹起
		{
			KeySta[keyout][i] = 1;              //改变该按键状态
      KeyDownTime[keyout][i] = 0;         //该按键按下时间清零
		}
	}
	keyout++;
	keyout&=0x03;                           //keyout到4自动清零
	switch(keyout)
	{
		case 0:KEY_OUT_4=1,KEY_OUT_1=0;break; //拉高当前输出脚 拉低下次输出脚
		case 1:KEY_OUT_1=1,KEY_OUT_2=0;break;
		case 2:KEY_OUT_2=1,KEY_OUT_3=0;break;
		case 3:KEY_OUT_3=1;KEY_OUT_4=0;break;
		default:break;
	}
}
