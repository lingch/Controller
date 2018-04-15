#include "uart.h"
#include "debug.h"

// void UartInit(void)		//9600bps@11.0592MHz
// {
// 	PCON &= 0x7F;		//�����ʲ�����(SM0D=0)
// 	SCON = 0x50;		//8λ����,�ɱ䲨����
// 	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
// 	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
// 	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
// 	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
// 	TL1 = 0xDC;		//�趨��ʱ��ֵ
// 	TH1 = 0xDC;		//�趨��ʱ����װֵ
// 	ET1 = 0;		//��ֹ��ʱ��1�ж�
// 	TR1 = 1;		//������ʱ��1

// 	debugStr("uart initialization OK");
// }

#define BAUD_RATE 9600
void UartInitUseT2(void)		//9600bps@11.0592MHz
{
	u16 reload = 0xff;

	SCON |= 0x40;		//Mode 1
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	
	AUXR &= 0x10;	//stop T2
	IE2 &= (~0x04);		//��ֹ��ʱ��2�ж�
	AUXR &= 0x08;	//T2 as timer, not counter
	AUXR |= 0x04;	//1T timer, not 12T timer
	reload = 65535 - MAIN_Fosc/4/BAUD_RATE;
	TL2 = reload & 0xff;;		//�趨��ʱ��ֵ
	TH2 = (reload >> 4) & 0xff;		//�趨��ʱ����װֵ
	AUXR |= 0x10;		//Start T2

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
