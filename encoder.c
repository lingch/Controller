#include "encoder.h"
#include "debug.h"

//totally 13 bit to form a 2262 word
#define ADDR_LEN_2262	8
#define DATA_LEN_2262	4
#define SYNC_LEN_2262	4
#define WORD_LEN_2262	(SYNC_LEN_2262*8+ADDR_LEN_2262*8+DATA_LEN_2262*8)

#define REPEAT_COUNT_2262	4

u8 DATA_STOCK_2262[WORD_LEN_2262];

//port definition
sbit OUTLET=P0^5;

u8 nRepeat;
u8 iSend;


void dumpStock(){
	u8 i;

	for(i=0;i<WORD_LEN_2262;++i){
		debug("%bx ",DATA_STOCK_2262[i] );
	}
}

void initEncoder(){
	OUTLET = 0;
	nRepeat = 0;
	iSend = WORD_LEN_2262;
}

//send wave form
void send2262(){
	if(nRepeat <= 0){
		OUTLET = 0;
		return;
	}
	
	//OUTLET = ~OUTLET;
	OUTLET = DATA_STOCK_2262[iSend];
	++iSend;
	if(iSend >= WORD_LEN_2262){
		//for next turn
		nRepeat--;
		iSend = 0;
	}
}

//load wave form
u8* loadWord(u8 *pAddr, u8 *pData){
	u8 i;
	u8 *pBit;

	pBit = DATA_STOCK_2262;
	
	//load synchronize bit
	pBit = loadBitSync(pBit);
	//load address
	for(i=0; i < ADDR_LEN_2262; ++i){
		pBit = loadBit(pBit,pAddr[i]);
	}
	//load data
	for(i=0; i < DATA_LEN_2262; ++i){
		pBit = loadBit(pBit,pData[i]);
	}

	//reset pointer
	iSend = 0;
	nRepeat = REPEAT_COUNT_2262;

	dumpStock();
	return pBit;
}

