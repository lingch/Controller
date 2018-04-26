#include "mgrstate.h"
#include "timeResolv.h"
#include "timertask.h"
#include "debug.h"
#include "timer.h"

#define LIGHT_ON 0
#define LIGHT_OFF 1

#define KEY_PRESSED 0
#define KEY_RELEASED 1


#define STATE_COUNT 4
#define LIGHT_COUNT 3

const u8 STATE_LIGHT[STATE_COUNT][LIGHT_COUNT] = {
	LIGHT_OFF,LIGHT_OFF,LIGHT_OFF,
	LIGHT_ON,LIGHT_OFF,LIGHT_OFF,
LIGHT_OFF,LIGHT_ON,LIGHT_OFF,
LIGHT_OFF,LIGHT_OFF,LIGHT_ON};


sbit Key1 = P1^0;
sbit LightHold1=P0^1;
sbit LightHold2=P0^2;
sbit LightHold3=P0^3;


enum EState state;

// typedef struct 
// {
// 	char pressed;
// 	TimerResolution tPress;
// } KeyPress;

void updateLight(){
	LightHold1 = STATE_LIGHT[state][0];
	LightHold2 = STATE_LIGHT[state][1];
	LightHold3 = STATE_LIGHT[state][2];
}

void mgrStateInit(){
	//key1.pressed = 0;

	state = Idle;
	updateLight();
}




void rotateState(){
	state = (state + 1) % STATE_COUNT;	//shift state

	//Light on/off
	updateLight();
}

void flashRunningLight(){
	P55 = ~P55;
}

enum EState getCurState(){
	return state;
}
