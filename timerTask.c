/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ
******************************************/

#include "timer.h"
#include <stdlib.h>
#define	Timer1_Reload	(MAIN_Fosc / 1000)		//Timer 1 中断频率, 1000次/秒

/*-------The timer structure--------*/
TimerResolution tNow;
void tInit(){
	tNow.sec = tNow.msec = 0;
}

/*-------The timer function--------*/



TimerTask *ttasks = NULL;

static u8 TIMER_TASK_ID = 0;

TimerTask* addTimerTask(TimerTask **ppTaskHead, TimerProc callback, u32 sec, u16 msec){
	TimerTask *ret = (TimerTask*)malloc(sizeof( TimerTask) / sizeof(u8));
	ret->id = TIMER_TASK_ID++;
	ret->proc = callback;
	ret->interval.sec = sec;
	ret->interval.msec = msec;
	ret->lastRun = tNow;

	if(*ppTaskHead == NULL){
		ret->prev = ret->next = NULL;
	}else{
		ret->next = *ppTaskHead;
		ret->prev = NULL;
		(*ppTaskHead)->prev = ret;
	}

	*ppTaskHead = ret;
	return ret;
}

void delTimerTask(TimerTask **ppTaskHead, TimerTask *p){
	if(*ppTaskHead == NULL){
		return;
	}

	//p is *ppTaskHead's only element
	if(p->prev == p->next == NULL){
		*ppTaskHead = NULL;
	}

	if(p->prev){
		//p is not the head
		((TimerTask*)(p->prev))->next = p->next;
	}else{
		//p is the head
		*ppTaskHead = p->next;
	}

	if(p->next){
		//p is not the tail
		((TimerTask*)(p->next))->prev=p->prev;
	}

	free(p);
}


