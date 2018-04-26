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

	timer->tNow.msec = 0;
	timer->tNow.sec = 0;
	timer->pTaskHead = NULL;
	timer->nextTimerTaskID = 0;
	timer->overflow = overflow;
	timer->timerReload = fsys / timer->overflow;

	//control function initialization
	timer->cStart = NULL;
	timer->cStop = NULL;

	debug("tInit timerReload=%lu, overflow=%u \r\n",timer->timerReload,timer->overflow);
}

TimerResolution tGetNow(Timer *timer)
{
	TimerResolution t;
	timer->cStop();
	t = timer->tNow;
	timer->cStart();

	return t;
}
void tSetNow(Timer *timer, TimerResolution tNow)
{
	timer->cStop();
	timer->tNow = tNow;
	timer->cStart();
}

TimerTask* addTimerTask(Timer *timer, TimerProc callback, u32 sec, u16 msec){
	
	Node *node;
	TimerTask *task;

	task = createTimerTask(sec,msec,callback);

TR1 = 0;
	node = addNode(timer->pTaskHead, task);
	timer->pTaskHead = node;
	
	//TODO: interup priority problem
	debug("task added, id=%bd, lastrun=%lu,%u \r\n",task->id,
		task->lastRun.sec,task->lastRun.msec);

TR1 = 1;
	return task;
}

void delTimerTask(Timer *timer, TimerTask *pTask){

	timer->pTaskHead = deleteNode(timer->pTaskHead,pTask);

	freeTimerTask(pTask);
	debugStr("task free");
}

void t1Stop(){
	TR1=0;
}
void t1Start(){
	TR1=1;
}

void	Timer1_init(void)
{
	tInit(&timer1, MAIN_Fosc, 100 );
	timer1.cStart = t1Start;
	timer1.cStop = t1Stop;

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
	TimerResolution tTmp;

	Node *pNode;
	TimerTask *pTask;
	TimerResolution delta;
	
	u8 taskCount;
	u32 prevSec;

	prevSec = timer->tNow.sec;
	//update timer time
	tTmp = tGetNow(timer);
	tTmp = tIncrease(tTmp, timer->overflow);
	tSetNow(timer,tTmp);

	if(timer->tNow.sec > prevSec){
	 	debug("after tNow updated(%lu,%u)\n",timer->tNow.sec,timer->tNow.msec);
	}
	//TODO: will this _interrupt function re-entrance?
	taskCount = 0;
	pNode = timer->pTaskHead;
	while(pNode){
		taskCount++;

		pTask = (TimerTask*)pNode->pData;
		delta = tSub(timer1.tNow,pTask->lastRun);

		if(tCmp(delta,pTask->interval) > 0){
			//time to run
			debug("task id=%bd, lastrun=%lu,%u, now=%lu,%u, delta=%lu,%u, interval=%lu,%u\n",
				(u8)pTask->id, (u32)pTask->lastRun.sec,(u16)pTask->lastRun.msec,
				(u32)timer1.tNow.sec,(u16)timer1.tNow.msec,
				(u32)delta.sec,(u16)delta.msec,
				(u32)pTask->interval.sec,(u16)pTask->interval.msec);
			debugStr("timer1 run callback");
			pTask->proc();

			//update last run timestamp
			pTask->lastRun = timer1.tNow;
		}

		pNode = pNode->next;
	} 
	//debug("loop through %d tasks\n", taskCount);
}

void timer1_int (void) interrupt TIMER1_VECTOR
{
	processTasks(&timer1);
}

void timer2_int (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}


