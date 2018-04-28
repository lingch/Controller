
#include "timer3.h"
#include "debug.h"

Timer timer3;

void t3Stop(){
	T4T3M &= ~0x08;
}
void t3Start(){
	t3EnableInt(1);
	T4T3M |= 0x08;
}
void t3EnableInt(u8 param){
	if(param){
		IE2 |= 0x20;
	}else{
		IE2 &= ~0x20;
	}
}

void t3Set12T(u8 param){
	if(param){
		T4T3M &= ~0x02;	//12T mode
	}else{
		T4T3M |=  0x02;	//1T mode
	}
}

void t3SetTH(u8 param){
	TH3 = param;
}

void t3SetTL(u8 param){
	TL3 = param;
}

void t3Init(u16 overflow)
{
	timer3.cStart = t3Start;
	timer3.cStop = t3Stop;
	timer3.cSet12T = t3Set12T;
	timer3.cSetTH = t3SetTH;
	timer3.cSetTL = t3SetTL;
	timer3.cEnableInt = t3EnableInt;
	
	tInit(&timer3, MAIN_Fosc , overflow);
	timerInit(&timer3);
}

void t3Interrupt (void) interrupt TIMER3_VECTOR
{
	// debugStr("aaa");
	processTasks(&timer3);
}

