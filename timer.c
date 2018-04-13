/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ?
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

	if (timer1.timerReload < 64)	{ // 如果用户设置值不合适， 则不启动定时器
		//TODO: error "Timer1设置的中断过快!"
	}
	else if ((timer1.timerReload/12) < 65536UL){	// 如果用户设置值不合适， 则不启动定时器
debugStr("timer1 initialization 1");
		TR1 = 0;	//停止计数
		//ET1 = 1;	//允许中断
		//PT1 = 1;	//高优先级中断
		//TMOD &= ~0x30;
		//TMOD |= (0 << 4);	//工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装
	//	TMOD |=  0x40;	//对外计数或分频
		//TMOD &= ~0x40;	//定时
	//	INT_CLKO |=  0x02;	//输出时钟
		//INT_CLKO &= ~0x02;	//不输出时钟
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

		TR1 = 1;	//开始运行

		debugStr("timer1 initialization OK");
	}
	else{
		//TODO: error "Timer1设置的中断过慢!"
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


