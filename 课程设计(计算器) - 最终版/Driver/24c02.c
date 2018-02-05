#include "24c02.h"

/*========================================================================*/
/*函数名称:void I2C_Start()
/*函数功能:I2C开始函数
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void I2C_Start()
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2CDelay();
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 0;
}

/*========================================================================*/
/*函数名称:void I2C_Stop()
/*函数功能:I2C停止函数
/*输入参数:无
/*输出参数:无
/*========================================================================*/
void I2C_Stop()
{
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SDA = 1;
	I2CDelay();
}

/*========================================================================*/
/*函数名称:bit I2C_Write(uchar dat)
/*函数功能:I2C总线写操作函数
/*输入参数:dat(待写入字节)
/*输出参数:返回从机的应答值  
/*========================================================================*/
bit I2C_Write(uchar dat)
{
	bit ack = 0;
	uchar mask;
	for(mask=0x80; mask!=0; mask>>=1)               //将mask按位与dat 从高位到低位依次判断dat每一位的值
	{
		if((mask&dat) == 0)                           //若值为0 则拉低SDA 若为1则拉高SDA
			I2C_SDA = 0;
		else
			I2C_SDA = 1;
		I2CDelay();
		I2C_SCL = 1;
		I2CDelay();
		I2C_SCL = 0;
	}
	I2C_SDA = 1;
	I2CDelay();
	I2C_SCL = 1;
	ack = I2C_SDA;
	I2CDelay();
	I2C_SCL = 0;

	return (~ack);
}

/*========================================================================*/
/*函数名称:uchar I2CReadACK()
/*函数功能:发送应答信号
/*输入参数:无
/*输出参数:返回读到的字节   
/*========================================================================*/
uchar I2CReadACK()
{
	uchar mask;
	uchar dat;
	I2C_SDA=1;                                  //确保主机释放SDA
	for(mask=0x80;mask!=0;mask>>=1)
	{
		I2CDelay();
		I2C_SCL=1;                                //拉高SCL
		if(I2C_SDA==0)
			dat&=~mask;                             //SDA为0时 dat对应位清零
		else
			dat|=mask;
		I2CDelay();
		I2C_SCL=0;                                //拉低SCL 使从机发送下一位
	}
	I2C_SDA=0;                                  //8位数据发送完后 拉低SDA 发送应答信号
	I2CDelay();
	I2C_SCL=1;                                  //拉高SCL
	I2CDelay();
	I2C_SCL=0;                                  //再拉低SCL 完成应答位 保持住总线
	return dat;
}

/*========================================================================*/
/*函数名称:uchar I2CReadACK()
/*函数功能:发送非应答信号
/*输入参数:无
/*输出参数:返回读到的字节   
/*========================================================================*/
uchar I2CReadNAK()
{
	uchar mask;
	uchar dat;
	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		I2CDelay();
		I2C_SCL=1;
		if(I2C_SDA==0)
			dat&=~mask;
		else
			dat|=mask;
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SCL=0;
	return dat;
}

/*========================================================================*/
/*函数名称:void E2P_Read(uchar *buf,uchar addr,uchar len)
/*函数功能:读取某地址一定长度的数据
/*输入参数: buf：数据接收指针 addr：EEPROM起始地址 len：读取数据长度   
/*输出参数:无
/*========================================================================*/
void E2P_Read(uchar *buf,uchar addr,uchar len)
{
	do                                                //通过寻址操作判断当前是否可进行读写
	{
		I2C_Start();
		if(I2C_Write(0x50<<1))                          //应答则跳出循环 非应答则继续查询
			break;
		I2C_Stop();
	}while(1);
	
	I2C_Write(addr);                                  //写入起始地址
	I2C_Start();                                      //发送重复起始信号
	I2C_Write((0x50<<1)|0x01);                        //寻址器件 后续为读操作
	while(len>1)                                      //连续读取len—1个字节
	{
		*buf++=I2CReadACK();                            //最后字节之前为读取操作+应答
		len--;
	}
	*buf=I2CReadNAK();                                //最后一个字节为读取操作+非应答
	I2C_Stop();
}

/*========================================================================*/
/*函数名称:void E2P_Write(uchar *buf,uchar addr,uchar len)
/*函数功能:向某地址写入一定长度数据
/*输入参数: buf：数据接收指针 addr：EEPROM起始地址 len：读取数据长度   
/*输出参数:无
/*========================================================================*/
void E2P_Write(uchar *buf,uchar addr,uchar len)
{
	while(len--)                                        //每写一个字节都要检测一次状态
	{
		do                                                //通过寻址操作判断当前是否可进行读写
	  {
			I2C_Start();
		  if(I2C_Write(0x50<<1))                          //应答则跳出循环 非应答则继续查询
			  break;
		  I2C_Stop();
	  }while(1);
		
		I2C_Write(addr++);                                //写入起始地址 地址依次+1
		I2C_Write(*buf++);                                //写入一个字节数据
		I2C_Stop();
  }
}