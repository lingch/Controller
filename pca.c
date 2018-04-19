#include "config.h"
#include "debug.h"

u32 cfCount = 0;

sbit PCA_INPUT=P1^2;

void pcaInit(){
	//default 12 divided to main system frequency
	CMOD |= 0x01; //ECF = 1;
	//ECCF0 = 1;enable interupt for PCA0
	CCAPM0 |= 0x31;	//both positive & nagative edge trigger for PCA0; 
	
	CR = 1;

	debugStr("PCA initialization OK");
}

u8 testPress(){
	if(PCA_INPUT == 1){
		return 1;
	}else{
		return 0;
	}
}

double measurePressTime(){
	return 12 * 0xffff / MAIN_Fosc * (cfCount + (CH<<8 + CL) / 0xffff);
}

void timer1_int (void) interrupt PCA_VECTOR{
	if(CF){
		cfCount++;
		CF = 0;
	}

	if(CCF0){
		if(testPress()){
			//pressed down
			cfCount = 0;
			CH=CL=0;
			debugStr("PCA pressed");
		}else{
			//release
			double sec = measurePressTime();

			debug("PCA released, last time=%f\n",sec);
		}
	}

	if(CCF1){

	}

}