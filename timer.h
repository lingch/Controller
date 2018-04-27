#ifndef _TIMER_H_
#define _TIMER_H_

#include "config.h"

#include	"STC15Fxxxx.H"
#include "timeResolv.h"
#include "timerTask.h"
#include "list.h"

typedef void (*PTRunControl)(void);
typedef void (*PTSet)(u8 param);

typedef struct {
	TimerResolution tNow;
	Node *pTaskHead;
	 u8 nextTimerTaskID;
	 u16 overflow;
	 u32 timerReload;

	 PTRunControl cStart;
	 PTRunControl cStop;
	 PTSet cSet12T;
	 PTSet cSetTH;
	 PTSet cSetTL;
	 PTSet cEnableInt;
} Timer;

void tInit(Timer *timer, u32 fsys, u16 overflow);
void timerInit(Timer *timer);

TimerResolution tGetNow(Timer *timer);
TimerTask* addTimerTask(Timer *timer, TimerProc callback, u32 sec, u16 msec);
void delTimerTask(Timer *timer, TimerTask *p);
void processTasks(Timer *timer);

#endif
