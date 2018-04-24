#include "uart.h"
#include "debug.h"

void UartInitUseT1(void)		//9600bps@11.0592MHz
{
	//S1_USE_P36P37();

	PCON &= 0x7F;		//�����ʲ�����(SM0D=0)
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;		//�趨��ʱ��ֵ
	TH1 = 0xDC;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

	debugStr("uart initialization OK");
}

#define BAUD_RATE 9600
void UartInitUseT2(void)		//9600bps@11.0592MHz
{
	u16 reload;
	
	PCON &= 0x7F;		//�����ʲ�����(SM0D=0)

	//SCON |= 0x80;		//Mode 1
	SCON |= 0x40;		//SM1=1
	AUXR |= 0x04;		//T2X12 = 1
	AUXR |= 0x01;		//S1ST2 = 1
	
	AUXR &= ~0x10;	//stop T2
	
	//AUXR &= ~0x0C;	//T2 as 1T timer, not counter

	reload = 0xffff - MAIN_Fosc/4/BAUD_RATE;
	RL_T2L = TL2 = reload & 0x00ff;		
	RL_T2H = TH2 = (reload  & 0xff00) >> 8;	
	//RL_T2L = TL2 = 0xDC;
	//RL_T2H = TH2 = 0xff;	
	//RL_TL1=0xDC;

	IE2 &= (~0x04);		//��ֹ��ʱ��2�ж�
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
