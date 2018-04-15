#include "uart.h"
#include "debug.h"

// void UartInit(void)		//9600bps@11.0592MHz
// {
// 	PCON &= 0x7F;		//波特率不倍速(SM0D=0)
// 	SCON = 0x50;		//8位数据,可变波特率
// 	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
// 	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
// 	TMOD &= 0x0F;		//清除定时器1模式位
// 	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
// 	TL1 = 0xDC;		//设定定时初值
// 	TH1 = 0xDC;		//设定定时器重装值
// 	ET1 = 0;		//禁止定时器1中断
// 	TR1 = 1;		//启动定时器1

// 	debugStr("uart initialization OK");
// }

#define BAUD_RATE 9600
void UartInitUseT2(void)		//9600bps@11.0592MHz
{
	u16 reload = 0xff;

	SCON |= 0x40;		//Mode 1
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	
	AUXR &= 0x10;	//stop T2
	IE2 &= (~0x04);		//禁止定时器2中断
	AUXR &= 0x08;	//T2 as timer, not counter
	AUXR |= 0x04;	//1T timer, not 12T timer
	reload = 65535 - MAIN_Fosc/4/BAUD_RATE;
	TL2 = reload & 0xff;;		//设定定时初值
	TH2 = (reload >> 4) & 0xff;		//设定定时器重装值
	AUXR |= 0x10;		//Start T2

	debugStr("uart initialization OK");
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}
