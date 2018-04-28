#ifndef _TIMER2_H_
#define _TIMER2_H_

#include "timer.h"

extern Timer timer2;

void t2Init(u16 overflow);
void t2EnableInt(u8 param);



#endif
