/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ
******************************************/

#include "timerTask.h"
#include <stdlib.h>
#include "debug.h"

#define	Timer1_Reload	(MAIN_Fosc / 1000)		//Timer 1 中断频率, 1000次/秒


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

