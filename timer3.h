#ifndef _TIMER3_H_
#define _TIMER3_H_

#include "timer.h"

extern Timer timer3;

void t3Init(u16 us, u16 overflow);
void t3EnableInt(u8 param);



#endif
