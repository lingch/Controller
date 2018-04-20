
#include "config.h"
#include "debug.h"
#include "TimerTask.h"
#include "timer.h"
#include "STC15Fxxxx.h"


TimerTask* taskRunningLight = NULL;

void keyInit(){
	IT0 = 0;	//both rising edge and down edge trigger
	EX0 = 1;
}


void onKeyDown(){
	taskRunningLight = addTimerTask(&timer1,flashRunningLight,0,100);
	P0 = 0x00;
}

void onKeyUp(){
	P0 = 0xff;
}


void keyInt (void) interrupt INT0_VECTOR{
	if(INT0){
		onKeyUp();
	}else{
		onKeyDown();
	}
}

