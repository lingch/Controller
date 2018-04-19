#include "config.h"
#include "debug.h"

u32 cfCount;

sbit PCA_INPUT=P1^1;

void pcaInit(){
	cfCount = 0;

	//default 12 divided to main system frequency
	CMOD |= 0x07; //ECF = 1;
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
	float ret = 12.0f * 0xffff / MAIN_Fosc;
	//float part = 1.0f * (u16)(CCAP0H<<8 + CCAP0L);// / 0xffff;
	return ret; // ret * (cfCount + );
}

void pcaInt (void) interrupt PCA_VECTOR{
	u8 a,b;
	if(CF){
		

		cfCount++;
		//debugStr("CF");
		CF = 0;
	}

	if(CCF0){
		

		debugStr("CCF0");
		if(testPress()){
			//pressed down
			cfCount = 0;
			//CH=CL=0;
			a = CCAP0H;b=CCAP0L;
			debug("PCA pressed, %u,%u\n",a,b);
		}else{
			//release
			float sec = measurePressTime();

			debug("PCA released, %u, %u, last time=%f\n",CCAP0H,CCAP0L,sec);
		}
		
		CCF0 = 0;
		
	}

	if(CCF1){
		CCF1 = 0;
	}

	CR = 1;
}