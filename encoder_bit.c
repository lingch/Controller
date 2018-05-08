#include "config.h"
#include "debug.h"
#include "STC15FXXXX.h"
#include "encoder_bit.h"

//port definition
sbit OUTLET=P0^5;

//wave pattern definition
u8 BIT0 = 0x88;
u8 BIT1 = 0xee;
u8 BITF = 0x8e;
u8 BITSYNC[4] = {0x00,0x00,0x00,0x80};	//byte order is from 3 to 0
										//byte 3 is the first byte

//wave stock
u8 BIT2262[4];

u8 nByte;	//byte count is wave stock
char idxBit;	//bit index to indicate wave H or L

//call each flip time(4a in document)
//return 1 when there is more wave to flip
//return 0 when there is no more wave to flip
//which means caller should load next bit 
u8 send(){
	if((BIT2262[nByte]) & (0x80 >> idxBit)){
		OUTLET = 1;
	}else{
		OUTLET = 0;
	}
	
	--idxBit;
	if(idxBit < 0){
		idxBit = 7;
		--nByte;
		if(nByte < 0){
			return 0;
		}
	}

	return 1;
}

void loadBit0(){
	BIT2262[0] = BIT0;
	nByte = 0;
	idxBit = 7;
}

void loadBit1(){
	BIT2262[0] = BIT1;
	nByte = 0;
	idxBit = 7;
}

void loadBitF(){
	BIT2262[0] = BITF;
	nByte = 0;
	idxBit = 7;
}

void loadBitSync(){
	BIT2262[0] = BITSYNC[0];
	BIT2262[1] = BITSYNC[1];
	BIT2262[2] = BITSYNC[2];
	BIT2262[3] = BITSYNC[3];
	nByte = 3;
	idxBit = 7;
}

