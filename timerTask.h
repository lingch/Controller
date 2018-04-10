#ifndef _TIMER_TASK_H_
#define _TIMER_TASK_H_

#include "config.h"
#include "TimeResolv.h"

typedef void (*TimerProc)(void);

typedef struct {
    u8 id;
	TimerProc proc;
	TimerResolution interval;
	TimerResolution lastRun;
}TimerTask;

TimerTask* createTimerTask(u32 sec, u16 msec,TimerProc callback);
void freeTimerTask(TimerTask *pTask);

#endif
