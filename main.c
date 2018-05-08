#include	"STC15Fxxxx.H"
#include "uart.h"
#include "timer.h"
#include "delay.h"
#include "debug.h"
#include "pca.h"
#include "key.h"
#include "mgrState.h"
#include "mem.h"
#include "timer1.h"
#include "timer2.h"
#include "timer3.h"
#include <stdio.h>
#include "encoder.h"

#define DIS_DOT		0x20
#define DIS_BLACK	0x10
#define DIS_		0x11

/****************************** 用户定义宏 ***********************************/

#define		Timer0_Reload	(65536UL -(MAIN_Fosc / 1000))		//Timer 0 中断频率, 1000次/秒

/********************** 主函数 ************************/
u8 working;


void mainInit(){
	working = 0;
	
	EA = 1;		//允许总中断
	
	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
	P1M1 = 0;	P1M1 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P0=0xff;
	P1=0xff;

	debugStr("main process initialization OK");
}

TimerTask* taskTest = NULL;

u8 i=0x01;
void shiftP55(){
	P55 = ~P55;
	
	i = i <<1;
	if(i> 0x08)
		i=0x01;
	P0 = i & 0x0f;
	
	P0 &= ~0x10;
	delay_ms(5);
	P0 |= 0x10;
}
void main(void)
{
	memInit();
	timerTaskInit();
	t2Init(0,1000); // uart will reconfigure the timer step
	uartInit(&timer2);
	debugInit();
	//pcaInit();
	t1Init(10000 ,100); // 10ms step timer
	t3Init(90, 100);	// 90us step timer, for 2262 encoder use
	keyInit();
	mgrStateInit();
	mainInit();

	debugStr("all initialization done, main process started");

taskTest = addTimerTask(&timer1, shiftP55, 1,0);
//send2262(0x01,0x02);
	
working=1;
	while(1){
		if(!working){
			debugStr("going to sleep");
			PCON |= 0x02;	//Sleep
			_nop_();
			_nop_();
			_nop_();
			debugStr("awake from sleep");
		}
	}
	
}


