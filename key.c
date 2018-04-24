
#include "config.h"
#include "debug.h"
#include "TimerTask.h"
#include "timer.h"
#include "STC15Fxxxx.h"

#include "mgrstate.h"


TimerTask* taskRunningLight = NULL;
TimerTask* taskRotateState = NULL;

void keyInit(){
	IT0 = 0;	//both rising edge and down edge trigger
	EX0 = 1;
}


void onKeyDown(){
	//taskRunningLight = addTimerTask(&timer1,flashRunningLight,0,100);
	taskRotateState = addTimerTask(&timer1, rotateState, 3,0);
	P0 = 0x00;
}

void onKeyUp(){
	delTimerTask(&timer1,taskRotateState);
	P0 = 0xff;
}


void keyInt (void) interrupt INT0_VECTOR{
	if(INT0){
		onKeyUp();
	}else{
		onKeyDown();
	}
}

/********************* INT0???? *************************/
// void INT0_int (void) interrupt INT0_VECTOR		//??????????
// {
// 	return;
// 	debugStr("detected key press");
	
// 	working = 1;

// 	if(key1.pressed){
// 		debugStr("key already pressed");
// 		return;	//already pressed? should not be possible
// 		//TODO: add a debug warning
// 	}

// 	key1.pressed = 1;
// 	key1.tPress = tGetNow(&timer1);

// 	taskDetectKeyRelease = addTimerTask(&timer1,detectKeyRelease,0,10);
// 	taskRotateState = addTimerTask(&timer1, rotateState, 3,0);

// }
