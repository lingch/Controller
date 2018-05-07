#include "config.h"
#include "debug.h"
#include "timer2.h"
#include "STC15FXXXX.h"

sbit OUTLET=P1^0;

u8 BIT0 = 0x88;
u8 BIT1 = 0xee;
u8 BITF = 0x8e;
u8 BITSYNC[4] = {0x00,0x00,0x00,0x80};
u8 BIT2262[4];

u8 nByte;
char nBit;

TimerTask* taskSend = NULL;
Timer *encTimer;

void encoderInit(Timer *timer){
	encTimer = timer;
}

static void _end(){
	delTimerTask(encTimer,taskSend);
	taskSend = NULL;
}

void _send(){
	if((BIT2262[nByte]) & (0x80 >> nBit)){
		OUTLET = 1;
	}else{
		OUTLET = 0;
	}
	
	--nBit;
	if(nBit < 0){
		nBit = 7;
		--nByte;
		if(nByte < 0){
			_end();
		}
	}
}

static void _start(){
	taskSend = addTimerTask(encTimer, _send, 0 ,1);
}


void sendBit0(){
	BIT2262[0] = BIT0;
	nByte = 0;
	nBit = 7;

	_start();
}

void sendBit1(){
	BIT2262[0] = BIT1;
	nByte = 0;
	nBit = 7;

	_start();
}

void sendBitF(){
	BIT2262[0] = BITF;
	nByte = 0;
	nBit = 7;

	_start();
}

void sendBitSync(){
	BIT2262[0] = BITSYNC[0];
	BIT2262[1] = BITSYNC[1];
	BIT2262[2] = BITSYNC[2];
	BIT2262[3] = BITSYNC[3];
	nByte = 3;
	nBit = 7;

	_start();
}




