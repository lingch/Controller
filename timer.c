/*************	����˵��	**************
��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ?
******************************************/

#include "timer.h"
#include <stdlib.h>
#include "debug.h"

/*-------The timer structure--------*/

Timer timer1;
Timer timer2;

void tInit(Timer *timer, u32 fsys, u16 overflow){
	timer->tNow.sec = timer->tNow.msec = 0;
	timer->pTaskHead = NULL;
	timer->nextTimerTaskID = 0;
	timer->overflow = overflow;
	timer->timerReload = fsys / timer->overflow;

	debug("init overflow=%u,%u\n",timer->timerReload,timer->overflow);
}

TimerResolution tGetNow(Timer *timer)
{
	return timer->tNow;
}

TimerTask* addTimerTask(Timer *timer, TimerProc callback, u32 sec, u16 msec){
	Node *node;
	TimerTask *task = createTimerTask(sec,msec,callback);
	debugStr("task created");
	task->lastRun = tGetNow(timer);
	node = addNode(timer->pTaskHead, task);
	debug("new added node=%08x\n",node);
	timer->pTaskHead = node;
	debugStr("task added");

	return task;
}

void delTimerTask(Timer *timer, TimerTask *pTask){

	timer->pTaskHead = deleteNode(timer->pTaskHead,pTask);

	freeTimerTask(pTask);
	debugStr("task free");
}


void	Timer1_init(void)
{
	tInit(&timer1, MAIN_Fosc, 100 );

	if (timer1.timerReload < 64)	{ // ����û�����ֵ�����ʣ� ��������ʱ��
		//TODO: error "Timer1���õ��жϹ���!"
	}
	else if ((timer1.timerReload/12) < 65536UL){	// ����û�����ֵ�����ʣ� ��������ʱ��
debugStr("timer1 initialization 1");
		TR1 = 0;	//ֹͣ����
		//ET1 = 1;	//�����ж�
		//PT1 = 1;	//�����ȼ��ж�
		//TMOD &= ~0x30;
		//TMOD |= (0 << 4);	//����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ
	//	TMOD |=  0x40;	//����������Ƶ
		//TMOD &= ~0x40;	//��ʱ
	//	INT_CLKO |=  0x02;	//���ʱ��
		//INT_CLKO &= ~0x02;	//�����ʱ��
debugStr("timer1 initialization 2");
		if (timer1.timerReload < 65536UL){debugStr("timer1 initialization 3");
			AUXR |=  0x40;	//1T mode
			TH1 = (u8)((65536UL - timer1.timerReload) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload) % 256);
		}
		else{debugStr("timer1 initialization 4");
			AUXR &= ~0x40;	//12T mode
			TH1 = (u8)((65536UL - timer1.timerReload/12) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload/12) % 256);
		}

		TR1 = 1;	//��ʼ����

		debugStr("timer1 initialization OK");
	}
	else{
		//TODO: error "Timer1���õ��жϹ���!"
	}
}

void	Timer2_init(void)
{
	tInit(&timer2, MAIN_Fosc , 11111 /*90us*/);
}

void processTasks(Timer *timer){
	Node *pNode;
	TimerTask *pTask;
	TimerResolution delta;
	
	int taskCount = 0;
	int sec1 = timer->tNow.sec;

	timer->tNow = tIncrease(timer->tNow, timer->overflow);
	if(timer->tNow.sec > sec1)
		debug("after tNow updated(%ld,%d)\n",timer->tNow.sec,timer->tNow.msec);
	//TODO: will this _interrupt function re-entrance?
	pNode = timer->pTaskHead;
	while(pNode){
		taskCount++;

		pTask = (TimerTask*)pNode->pData;
		delta = tSub(timer1.tNow,pTask->lastRun);
		if(tCmp(delta,pTask->interval)){
			//time to run
			debugStr("run timer callback");
			pTask->proc();
		}

		pNode = pNode->next;
	} 
	//debug("loop through %d tasks\n", taskCount);
}

void timer1_int (void) interrupt TIMER1_VECTOR
{
	//debugStr("timer1 int enter");
	processTasks(&timer1);
	//debugStr("timer1 int exit");
}

void timer2_int (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}


