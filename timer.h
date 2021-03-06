#ifndef _TIMER_H_
#define _TIMER_H_

#include "config.h"

#include	"STC15Fxxxx.H"
#include "timeResolv.h"
#include "timerTask.h"
#include "list.h"

typedef void (*PTRunControl)(void);

typedef struct {
	TimerResolution tNow;
	Node *pTaskHead;
	 u8 nextTimerTaskID;
	 u16 overflow;
	 u32 timerReload;

	 PTRunControl cStart;
	 PTRunControl cStop;
} Timer;

extern Timer timer1;
extern Timer timer2;

void	Timer1_init(void);
TimerResolution tGetNow(Timer *timer);
TimerTask* addTimerTask(Timer *timer, TimerProc callback, u32 sec, u16 msec);
void delTimerTask(Timer *timer, TimerTask *p);


#endif
