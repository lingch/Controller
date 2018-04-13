#include "timeResolv.h"
#include <stdlib.h>
#include "debug.h"

TimerResolution tIncrease(TimerResolution t, u16 msecOverflow){
	++t.msec;
	if(t.msec >= msecOverflow){
		//debug("overflow, sec = %lu, msec = %u, overflow=%u\n",t.sec,t.msec,msecOverflow);
		t.msec = 0;
		++t.sec;
	}

	return t;
}
char tCmp(TimerResolution t1, TimerResolution t2){
	if(t1.sec > t2.sec){
		return 1;
	}
	else if(t1.sec == t2.sec){
		if(t1.msec>t2.msec){
			return 1;
		}else if(t1.msec == t2.msec){
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}
TimerResolution tSub(TimerResolution t1, TimerResolution t2){
	TimerResolution ret;
	ret.sec = t1.sec - t2.sec;
	ret.msec = t1.msec - t2.msec;
	return ret;
}

