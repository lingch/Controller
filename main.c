/*---------------------------------------------------------------------*/
/* --- STC MCU International Limited ----------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/


/*************	本程序功能说明	**************

用STC的MCU的IO方式控制74HC595驱动8位数码管。

用户可以修改宏来选择时钟频率.

显示效果为: 左边为INT0(SW17)中断计数, 右边为INT1(SW18)中断计数, 计数范围为0~255.

由于按键是机械按键, 按下有抖动, 而本例程没有去抖动处理, 所以按一次有多个计数也是正常的.

******************************************/

#define MAIN_Fosc		11059200L	//定义主时钟

#include	"STC15Fxxxx.H"
#include "uart.h"
#include "timer.h"
#include "delay.h"

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

void detectKeyRelease();
void rotateState();

u8 working = 1;

void main(void)
{
	int i;
	
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
	
	for(i=0;i<10;i++){
		P55 = ~P55;
		delay_ms(100);
	}
	
	//PCON |= 0x02;	//Sleep
	
	//wake up here
	//	delay_ms(1);
	//P10 = 1;

	working = 0;
	
	while(1){

		P55 = ~P55;

		if(!key1.pressed)

		delay_ms(100);

		if(!working){
			PCON |= 0x02;	//Sleep
		}
	}
	
}

/********************* INT0中断函数 *************************/
void INT0_int (void) interrupt INT0_VECTOR		//进中断时已经清除标志
{
	working = 1;

	if(key1.pressed){
		return;	//already pressed? should not be possible
		//TODO: add a debug warning
	}

	key1.pressed = 1;
	key1.tPress = timer1GetNow();

	taskDetectKeyRelease = addTimerTask(&timer1.pTaskHead,detectKeyRelease,0,10);
	taskRotateState = addTimerTask(&timer1.pTaskHead, rotateState, 3,0);

}

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
		//detected release
		key1.pressed = 0;

		delTimerTask(&timer1.pTaskHead,taskRotateState);
		delTimerTask(&timer1.pTaskHead,taskDetectKeyRelease);
	}
}

