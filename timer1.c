
#include "timer1.h"
#include "debug.h"

Timer timer1;

void t1Stop(){
	TR1=0;
}
void t1Start(){
	TR1=1;
}

void t1Init(void)
{
	tInit(&timer1, MAIN_Fosc, 100 );
	timer1.cStart = t1Start;
	timer1.cStop = t1Stop;

	if (timer1.timerReload < 64)	{ // 如果用户设置值不合适， 则不启动定时器
		debugStr("Timer1 init too fast");
		PCON |= 0x02; //sleep
	}
	else if ((timer1.timerReload/12) < 65536UL){	// 如果用户设置值不合适， 则不启动定时器

		TR1 = 0;	//停止计数
		ET1 = 1;	//允许中断
		PT1 = 1;	//高优先级中断

		if (timer1.timerReload < 65536UL){
			debug("timer1 1T mode ");
			AUXR |=  0x40;	//1T mode
			TH1 = (u8)((65536UL - timer1.timerReload) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload) % 256);
		}
		else{
			debug("timer1 12T mode ");
			AUXR &= ~0x40;	//12T mode
			TH1 = (u8)((65536UL - timer1.timerReload/12) / 256);
			TL1 = (u8)((65536UL - timer1.timerReload/12) % 256);
		}

		TR1 = 1;	//开始运行

		debug("initialization OK\n");
	}
	else{
		debugStr("Timer1 init too slow");
		PCON |= 0x02; //sleep
	}
}

void t1Interrupt (void) interrupt TIMER1_VECTOR
{
	processTasks(&timer1);
}

