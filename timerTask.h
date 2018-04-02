#ifndef _TIMER_TASK_H_
#define _TIMER_TASK_H_

#include "config.h"

#include	"STC15Fxxxx.H"


typedef void (*TimerProc)(void);

typedef struct {
    u8 id;
	TimerProc proc;
	TimerResolution interval;
	TimerResolution lastRun;
	void *prev;
	void *next;
}TimerTask;

TimerTask* addTimerTask(TimerTask **ppTaskHead, TimerProc callback, u32 sec, u16 msec);
void delTimerTask(TimerTask **ppTaskHead, TimerTask *p);

#endif
