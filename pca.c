#include "config.h"
#include "debug.h"

u32 cfCount;

sbit PCA_INPUT=P1^1;
sbit PCA_INDICATOR=P0^0;

void pcaInit(){
	cfCount = 0;

	//default 12 divided to main system frequency
	CMOD |= 0x01; //ECF = 1;
	//ECCF0 = 1;enable interupt for PCA0
	CCAPM0 |= 0x31;	//both positive & nagative edge trigger for PCA0; 

	CR = 1;

	debugStr("PCA initialization OK");
}

u8 testPress(){
	if(PCA_INPUT == 1){
		return 0;
	}else{
		return 1;
	}
}

float measurePressTime(){
	float ret = 12.0f * 0xffff * cfCount / MAIN_Fosc;
	float part = 12.0f * (CCAP0H << 8 | CCAP0L) / MAIN_Fosc;
	return ret + part; // ret * (cfCount + );
}

u8 nStatus = 3;
u8 idxStatus = 0;
void pcaInt (void) interrupt PCA_VECTOR{
	if(CF){
		CF = 0;

		idxStatus = cfCount / 50 % nStatus;

		cfCount++;
	}

	if(CCF0){
		CCF0 = 0;

		debugStr("CCF0");
		if(testPress()){
			//pressed down
			cfCount = 0;
			CH=CL=0;

			debug("PCA pressed\n");
		}else{
			//release
			float sec = measurePressTime();

			debug("PCA released, last time=%f\n",sec);
		}
		
		
		
	}

	if(CCF1){
		CCF1 = 0;
	}

}