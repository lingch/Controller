
#include "timer2.h"
#include "debug.h"

Timer timer2;

void t2Stop(){
	AUXR &= ~0x10;
}
void t2Start(){
	AUXR |= 0x10;
}

void t2Init(void)
{
	tInit(&timer2, MAIN_Fosc , 11111 /*90us*/);
	timer2.cStart = t2Start;
	timer2.cStop = t2Stop;

	if (timer2.timerReload < 64)	{ // 如果用户设置值不合适， 则不启动定时器
		debugStr("Timer2 init too fast");
		PCON |= 0x02; //sleep
	}
	else if ((timer2.timerReload/12) < 65536UL){	// 如果用户设置值不合适， 则不启动定时器

		t2Stop();
		ET1 = 1;	//允许中断
		PT1 = 1;	//高优先级中断

		if (timer2.timerReload < 65536UL){
			debug("timer1 1T mode ");
			AUXR |=  0x04;	//1T mode
			TH2 = (u8)((65536UL - timer2.timerReload) / 256);
			TL2 = (u8)((65536UL - timer2.timerReload) % 256);
		}
		else{
			debug("timer1 12T mode ");
			AUXR &= ~0x04;	//12T mode
			TH2 = (u8)((65536UL - timer2.timerReload/12) / 256);
			TL2 = (u8)((65536UL - timer2.timerReload/12) % 256);
		}

		t2Start();

		debug("initialization OK\n");
	}
	else{
		debugStr("Timer2 init too slow");
		PCON |= 0x02; //sleep
	}
}

void t2Interrupt (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}

