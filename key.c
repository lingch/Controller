
#include "config.h"
#include "debug.h"

void keyInit(){
	IT0 = 0;	//both rising edge and down edge trigger
	EX0 = 1;
}

void keyInt (void) interrupt INT0_VECTOR{
	P0 = ~P0;
}