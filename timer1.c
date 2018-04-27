
#include "timer1.h"
#include "debug.h"

Timer timer1;

void t1Stop(){
	TR1=0;
}
void t1Start(){
	ET1 = 1;	//允许中断
	TR1=1;
}
void t1Set12T(u8 param){
	if(param){
		AUXR &= ~0x40;	//12T mode
	}else{
		AUXR |=  0x40;	//1T mode
	}
}

void t1SetTH(u8 param){
	TH1 = param;
}

void t1SetTL(u8 param){
	TL1 = param;
}

void t1Init(void)
{
	timer1.cStart = t1Start;
	timer1.cStop = t1Stop;
	timer1.cSet12T = t1Set12T;
	timer1.cSetTH = t1SetTH;
	timer1.cSetTL = t1SetTL;
	
	tInit(&timer1, MAIN_Fosc, 100 );
	timerInit(&timer1);
}

void t1Interrupt (void) interrupt TIMER1_VECTOR
{
	processTasks(&timer1);
}

