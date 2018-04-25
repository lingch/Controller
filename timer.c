/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ?
******************************************/

#include "config.h"
#include "timer.h"
#include "timerTask.h"
#include "debug.h"

/*-------The timer structure--------*/

Timer timer1;
Timer timer2;

void tInit(Timer *timer, u32 fsys, u16 overflow){
	timerTaskInit();

	timer->tNow.sec = timer->tNow.msec = 0;
	timer->pTaskHead = NULL;
	timer->nextTimerTaskID = 0;
	timer->overflow = overflow;
	timer->timerReload = fsys / timer->overflow;

	debug("tInit timerReload=%u, overflow=%u \r\n",timer->timerReload,timer->overflow);
}

TimerResolution tGetNow(Timer *timer)
{
	return timer->tNow;
}

TimerTask* addTimerTask(Timer *timer, TimerProc callback, u32 sec, u16 msec){
	Node *node;
	TimerTask *task = createTimerTask(sec,msec,callback);
	task->lastRun = tGetNow(timer);
	node = addNode(timer->pTaskHead, task);
	timer->pTaskHead = node;
	//TODO: interup priority problem
	debugStr("11111111111111111111111111");
	debug("task added, id=%bd, lastrun=%lu,%u \r\n",task->id,
		task->lastRun.sec,task->lastRun.msec);

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
		debugStr("Timer1 init too fast");
		PCON |= 0x02; //sleep
	}
	else if ((timer1.timerReload/12) < 65536UL){	// 如果用户设置值不合适， 则不启动定时器

		TR1 = 0;	//停止计数
		ET1 = 1;	//允许中断
		PT1 = 1;	//高优先级中断

		if (timer1.timerReload < 65536UL){
			debug("timer1 1T mode ");
			AUXR |=  0x40;	//1T mode
			TH1 = (u8)((65536UL - timer1.timerReload) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload) % 256);
		}
		else{
			debug("timer1 12T mode ");
			AUXR &= ~0x40;	//12T mode
			TH1 = (u8)((65536UL - timer1.timerReload/12) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload/12) % 256);
		}

		TR1 = 1;	//开始运行

		debug("initialization OK\n");
	}
	else{
		debugStr("Timer1 init too slow");
		PCON |= 0x02; //sleep
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
	
	u8 taskCount = 0;
	u32 sec1 = timer->tNow.sec;

	timer->tNow = tIncrease(timer->tNow, timer->overflow);
	// if(timer->tNow.sec > sec1)
	// 	debug("after tNow updated(%lu,%u)\n",timer->tNow.sec,timer->tNow.msec);

	//TODO: will this _interrupt function re-entrance?
	pNode = timer->pTaskHead;
	while(pNode){
		taskCount++;

		pTask = (TimerTask*)pNode->pData;
		delta = tSub(timer1.tNow,pTask->lastRun);

		if(tCmp(delta,pTask->interval)){
			//time to run
			debug("task id=%x, lastrun=%lu,%u, now=%lu,%u, delta=%lu,%u, interval=%lu,%u\n",
				(u8)pTask->id, (u32)pTask->lastRun.sec,(u16)pTask->lastRun.msec,
				(u32)timer1.tNow.sec,(u16)timer1.tNow.msec,
				(u32)delta.sec,(u16)delta.msec,
				(u32)pTask->interval.sec,(u16)pTask->interval.msec);
			debugStr("timer1 run callback");
			pTask->proc();
		}

		pNode = pNode->next;
	} 
	//debug("loop through %d tasks\n", taskCount);
}

void timer1_int (void) interrupt TIMER1_VECTOR
{
	//debugStr("timer int");
	//debugStr("timer1 int enter");
	processTasks(&timer1);
	//debugStr("timer1 int exit");
}

void timer2_int (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}


