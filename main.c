#include	"STC15Fxxxx.H"
#include "uart.h"
#include "timer.h"
#include "delay.h"
#include "debug.h"
#include "pca.h"
#include "key.h"
#include "mgrState.h"
#include "mem.h"

#include <stdio.h>

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
	
	P0M1 = 0;	P0M0 = 1;	//设置推挽模式
	P1M1 = 0;	P1M1 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P0=0xff;
	P1=0xff;

	debugStr("main process initialization OK");
}

void main(void)
{
	memInit();
	UartInit(); 
	debugInit();
	//pcaInit();
	Timer1_init();
	//Timer2_init();
	keyInit();
	mgrStateInit();
	mainInit();

	debugStr("all initialization done, main process started");

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


