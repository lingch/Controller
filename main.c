#include	"STC15Fxxxx.H"
#include "uart.h"
#include "timer.h"
#include "delay.h"
#include "debug.h"
#include"pca.h"

#include <stdio.h>

#define DIS_DOT		0x20
#define DIS_BLACK	0x10
#define DIS_		0x11

/****************************** 用户定义宏 ***********************************/

#define		Timer0_Reload	(65536UL -(MAIN_Fosc / 1000))		//Timer 0 中断频率, 1000次/秒

/********************** 主函数 ************************/

enum EState {Hold1=0, Hold2, Hold3};

sbit Key1 = P1^0;
sbit LightHold1=P0^1;
sbit LightHold2=P0^2;
sbit LightHold3=P0^3;

#define LIGHT_ON 0
#define LIGHT_OFF 1

#define KEY_PRESSED 0
#define KEY_RELEASED 1

enum EState state;

typedef struct 
{
	char pressed;
	TimerResolution tPress;
} KeyPress;

KeyPress key1;
TimerTask* taskRotateState = NULL;
TimerTask* taskDetectKeyRelease = NULL;
TimerTask* taskRunningLight = NULL;

void detectKeyRelease();
void rotateState();

u8 working;


void rotateState(){
	state = (state + 1) % 3;	//shift state

	//Light on/off
	switch(state){
		case Hold1:
			LightHold1 = LIGHT_ON;
			LightHold2 = LightHold3 = LIGHT_OFF;
			break;
		case Hold2:
			LightHold2 = LIGHT_ON;
			LightHold1 = LightHold3 = LIGHT_OFF;
			break;
		case Hold3:
			LightHold3 = LIGHT_ON;
			LightHold1 = LightHold2 = LIGHT_OFF;
			break;
		default:
			break;
	}
}

void detectKeyRelease(){
	if(key1.pressed ==1 && Key1 == KEY_RELEASED){
		
		debugStr("detected key release");
		
		//detected release
		key1.pressed = 0;

		delTimerTask(&timer1,taskRotateState);
		delTimerTask(&timer1,taskDetectKeyRelease);
	}
}

void flashRunningLight(){
	P55 = ~P55;
}


void mainInit(){
	key1.pressed = 0;
	working = 0;

	EA = 1;		//允许总中断
	IE0  = 0;	//外中断0标志位
	EX0 = 1;	//INT0 Enable
	IT0 = 1; //down edge trigger
	
	state = Hold1;
	
	P0M1 = 0;	P0M0 = 1;	//设置推挽模式
	P1M1 = 0;	P1M1 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P0=0x01;
	P1=0xff;

	debugStr("main process initialization OK");
}

void main(void)
{
	UartInit(); 
	pcaInit();
	Timer1_init();
	//Timer2_init();
	mainInit();

	debugStr("all initialization done, main process started");
	
	taskRunningLight = addTimerTask(&timer1,flashRunningLight,0,100);
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

/********************* INT0中断函数 *************************/
void INT0_int (void) interrupt INT0_VECTOR		//进中断时已经清除标志
{
	debugStr("detected key press");
	
	working = 1;

	if(key1.pressed){
		debugStr("key already pressed");
		return;	//already pressed? should not be possible
		//TODO: add a debug warning
	}

	key1.pressed = 1;
	key1.tPress = tGetNow(&timer1);

	taskDetectKeyRelease = addTimerTask(&timer1,detectKeyRelease,0,10);
	taskRotateState = addTimerTask(&timer1, rotateState, 3,0);

}
