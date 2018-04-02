#ifndef _TIMER_H_
#define _TIMER_H_

#include "config.h"

#include	"STC15Fxxxx.H"
#include "timeResolv.h"
#include "timerTask.h"

typedef struct {
	TimerResolution tNow;
	TimerTask *pTaskHead;
	 u8 nextTimerTaskID;
	 u16 timerReload;
} Timer;

extern Timer timer1;
extern Timer timer2;

TimerResolution timer1GetNow();

void	Timer1_init(void);


#endif
