#ifndef _TIMER1_H_
#define _TIMER1_H_

#include "timer.h"

extern Timer timer1;

void t1Init( u16 us, u16 overflow);
void t1EnableInt(u8 param);


#endif
