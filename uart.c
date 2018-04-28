#include "uart.h"
#include "debug.h"

#define BAUD_RATE 9600

void uartInit(Timer *timer)		//9600bps@11.0592MHz
{
	u16 reload;
	
	PCON &= 0x7F;		//�����ʲ�����(SM0D=0)
	//SCON |= 0x80;		//Mode 1
	SCON |= 0x40;		//SM1=1
	AUXR |= 0x01;		//S1ST2 = 1
	
	timer->cStop();
	//AUXR &= ~0x0C;	//T2 as 1T timer, not counter
	timer->cSet12T(0);
	reload = 0xffff - MAIN_Fosc/4/BAUD_RATE;
	timer->cSetTH((reload  & 0xff00) >> 8);
	timer->cSetTL(reload & 0x00ff);		

	timer->cEnableInt(0);
	timer->cStart();

	debugStr("uart initialization OK");
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //д���ݵ�UART���ݼĴ���
		while(TI == 0);
		TI = 0;
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}
