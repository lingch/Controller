/*************	����˵��	**************
��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ?
******************************************/

#include "timer.h"
#include <stdlib.h>
#include "debug.h"

/*-------The timer structure--------*/

Timer timer1;
Timer timer2;

void tInit(Timer *timer, u16 reloadCount){
	timer->tNow.sec = timer->tNow.msec = 0;
	timer->pTaskHead = NULL;
	timer->nextTimerTaskID = 0;
	timer->timerReload = reloadCount;
}

TimerResolution timer1GetNow()
{
	return timer1.tNow;
}

void	Timer1_init(void)
{


	if (timer1.timerReload < 64)	{ // ����û�����ֵ�����ʣ� ��������ʱ��
		//TODO: error "Timer1���õ��жϹ���!"
	}
	else if ((timer1.timerReload/12) < 65536UL){	// ����û�����ֵ�����ʣ� ��������ʱ��
		debugStr("Timer1_init");
		tInit(&timer1, MAIN_Fosc / 1000);
		
		TR1 = 0;	//ֹͣ����
	
		ET1 = 1;	//�����ж�
	//	PT1 = 1;	//�����ȼ��ж�
	// 	TMOD &= ~0x30;
	// 	TMOD |= (0 << 4);	//����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ
	// //	TMOD |=  0x40;	//����������Ƶ
	// 	TMOD &= ~0x40;	//��ʱ
	// //	INT_CLKO |=  0x02;	//���ʱ��
	// 	INT_CLKO &= ~0x02;	//�����ʱ��

	// 	if (timer1.timerReload < 65536UL){
	// 		AUXR |=  0x40;	//1T mode
	// 		TH1 = (u8)((65536UL - timer1.timerReload) / 256);
	// 		TL1 = (u8)((65536UL - timer1.timerReload) % 256);
	// 	}
	// 	else{
	// 		AUXR &= ~0x40;	//12T mode
	// 		TH1 = (u8)((65536UL - timer1.timerReload/12) / 256);
	// 		TL1 = (u8)((65536UL - timer1.timerReload/12) % 256);
	// 	}

		TR1 = 1;	//��ʼ����
	}
	else{
		//TODO: error "Timer1���õ��жϹ���!"
	}
}

void	Timer2_init(void)
{
	tInit(&timer2, MAIN_Fosc / 11111 /*90us*/);
}

void processTasks(Timer *timer){
	TimerTask *pTask;
	TimerResolution delta;

	timer->tNow = tIncrease(timer->tNow);
	
	//TODO: will this _interrupt function re-entrance?
	pTask = timer->pTaskHead;
	while(pTask){
		delta = tSub(timer1.tNow,pTask->lastRun);
		if(tCmp(delta,pTask->interval)){
			//time to run
			pTask->proc();
		}

		pTask = pTask->next;
	}
}

void timer1_int (void) interrupt TIMER1_VECTOR
{
	debugStr("timer1 int enter");
	processTasks(&timer1);
	debugStr("timer1 int exit");
}

void timer2_int (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}
