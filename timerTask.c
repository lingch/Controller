/*************	����˵��	**************
��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ
******************************************/

#include "timerTask.h"
#include <stdlib.h>
#include "debug.h"

#define	Timer1_Reload	(MAIN_Fosc / 1000)		//Timer 1 �ж�Ƶ��, 1000��/��


/*-------The timer function--------*/

u8 TIMER_TASK_ID = 0;

TimerTask* createTimerTask(u32 sec, u16 msec,TimerProc callback){
	TimerTask *ret = (TimerTask*)malloc(sizeof( TimerTask) / sizeof(u8));
TIMER_TASK_ID = 123;
	ret->id = TIMER_TASK_ID++;
	// ret->proc = callback;
	// ret->interval.sec = sec;
	// ret->interval.msec = msec;

	debug("11111=%x,%x\n",TIMER_TASK_ID,ret->id);
	return ret;
}

void freeTimerTask(TimerTask *pTask){
	free(pTask);
}

