/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ?
******************************************/

#include "config.h"
#include "timer.h"
#include "timerTask.h"
#include "debug.h"

/*-------The timer structure--------*/

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
	
	TimerResolution tNow;
	Node *node;
	TimerTask *task;

	task = createTimerTask(sec,msec,callback);
	tNow = tGetNow(timer);
	task->lastRun = tNow;

	timer->cStop();
	node = addNode(timer->pTaskHead, task);
	timer->pTaskHead = node;
	
	//TODO: interup priority problem
	debug("task added, id=%bd, lastrun=%lu,%u \r\n",task->id,
		task->lastRun.sec,task->lastRun.msec);

	timer->cStart();
	return task;
}

void delTimerTask(Timer *timer, TimerTask *pTask){

	timer->pTaskHead = deleteNode(timer->pTaskHead,pTask);

	freeTimerTask(pTask);
	debugStr("task free");
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
		delta = tSub(tTmp,pTask->lastRun);

		if(tCmp(delta,pTask->interval) > 0){
			//time to run
			debug("task id=%bd, lastrun=%lu,%u, now=%lu,%u, delta=%lu,%u, interval=%lu,%u\n",
				(u8)pTask->id, (u32)pTask->lastRun.sec,(u16)pTask->lastRun.msec,
				(u32)tTmp.sec,(u16)tTmp.msec,
				(u32)delta.sec,(u16)delta.msec,
				(u32)pTask->interval.sec,(u16)pTask->interval.msec);
			debugStr("timer run callback");
			pTask->proc();

			//update last run timestamp
			pTask->lastRun = tTmp;
		}

		pNode = pNode->next;
	} 
	//debug("loop through %d tasks\n", taskCount);
}
