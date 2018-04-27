
#include "timer2.h"
#include "debug.h"

Timer timer2;

void t2Stop(){
	AUXR &= ~0x10;
}
void t2Start(){
	IE2 |= 0x04;
	AUXR |= 0x10;
}
void t2Set12T(u8 param){
	if(param){
		AUXR &= ~0x04;	//12T mode
	}else{
		AUXR |=  0x04;	//1T mode
	}
}

void t2SetTH(u8 param){
	TH2 = param;
}

void t2SetTL(u8 param){
	TL2 = param;
}

void t2Init(void)
{
	timer2.cStart = t2Start;
	timer2.cStop = t2Stop;
	timer2.cSet12T = t2Set12T;
	timer2.cSetTH = t2SetTH;
	timer2.cSetTL = t2SetTL;
	
	tInit(&timer2, MAIN_Fosc , 11111 /*90us*/);
	timerInit(&timer2);
}

void t2Interrupt (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}

