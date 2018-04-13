#ifndef _TIMER_RESOLV_H_
#define _TIMER_RESOLV_H_

#include "config.h"

#include	"STC15Fxxxx.H"

typedef struct{
	u32 sec;
	u16 msec;
} TimerResolution;

TimerResolution tIncrease(TimerResolution t, u16 msecOverflow);
char tCmp(TimerResolution t1, TimerResolution t2);
TimerResolution tSub(TimerResolution t1, TimerResolution t2);

#endif
