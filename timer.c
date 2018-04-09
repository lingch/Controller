/*************	功能说明	**************
定时器1做16位自动重装, 中断频率为1000HZ?
******************************************/

#include "timer.h"
#include <stdlib.h>
#include "debug.h"

/*-------The timer structure--------*/

Timer timer1;
Timer timer2;

void tInit(Timer *timer, u16 reloadCount){
	timer->tNow.sec = timer->tNow.msec = 0;
	timer->pTaskHead = NULL;
	timer->nextTimerTaskID = 0;
	timer->timerReload = reloadCount;
}

TimerResolution timer1GetNow()
{
	return timer1.tNow;
}

void	Timer1_init(void)
{


	if (timer1.timerReload < 64)	{ // 如果用户设置值不合适， 则不启动定时器
		//TODO: error "Timer1设置的中断过快!"
	}
	else if ((timer1.timerReload/12) < 65536UL){	// 如果用户设置值不合适， 则不启动定时器
		debugStr("Timer1_init");
		tInit(&timer1, MAIN_Fosc / 1000);
		
		TR1 = 0;	//停止计数
	
		ET1 = 1;	//允许中断
	//	PT1 = 1;	//高优先级中断
	// 	TMOD &= ~0x30;
	// 	TMOD |= (0 << 4);	//工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装
	// //	TMOD |=  0x40;	//对外计数或分频
	// 	TMOD &= ~0x40;	//定时
	// //	INT_CLKO |=  0x02;	//输出时钟
	// 	INT_CLKO &= ~0x02;	//不输出时钟

	// 	if (timer1.timerReload < 65536UL){
	// 		AUXR |=  0x40;	//1T mode
	// 		TH1 = (u8)((65536UL - timer1.timerReload) / 256);
	// 		TL1 = (u8)((65536UL - timer1.timerReload) % 256);
	// 	}
	// 	else{
	// 		AUXR &= ~0x40;	//12T mode
	// 		TH1 = (u8)((65536UL - timer1.timerReload/12) / 256);
	// 		TL1 = (u8)((65536UL - timer1.timerReload/12) % 256);
	// 	}

		TR1 = 1;	//开始运行
	}
	else{
		//TODO: error "Timer1设置的中断过慢!"
	}
}

void	Timer2_init(void)
{
	tInit(&timer2, MAIN_Fosc / 11111 /*90us*/);
}

void processTasks(Timer *timer){
	TimerTask *pTask;
	TimerResolution delta;

	timer->tNow = tIncrease(timer->tNow);
	
	//TODO: will this _interrupt function re-entrance?
	pTask = timer->pTaskHead;
	while(pTask){
		delta = tSub(timer1.tNow,pTask->lastRun);
		if(tCmp(delta,pTask->interval)){
			//time to run
			pTask->proc();
		}

		pTask = pTask->next;
	}
}

void timer1_int (void) interrupt TIMER1_VECTOR
{
	debugStr("timer1 int enter");
	processTasks(&timer1);
	debugStr("timer1 int exit");
}

void timer2_int (void) interrupt TIMER2_VECTOR
{
	processTasks(&timer2);
}
