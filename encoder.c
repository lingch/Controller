#include "encoder.h"
#include "debug.h"

//function array to load different bit into positon
//map by EBitType
PLoadFunc bitTypeFunc[BIT_TYPE_COUNT] = {
	//!ORDER IS RELATED TO EBitType DEFINITION, DONT CHANGE!
	loadBitSync,
	loadBitF,
	loadBit0,
	loadBit1
};
void loadFirstBit(){
	PLoadFunc pLoadFun;
	idxBitSending = 0;
	
	pLoadFun = bitTypeFunc[bitTypeSeq[0]];
	pLoadFun();	//load bit
}

//load 2262 word in position
//EBitType in addr and data
//will load addr & data into bitTypeSeq
//will map to bitTypeFunc when sending
u8 loadWord(u8 *pAddr, u8 *pData){
	u8 i;
	u8 *pBit;

	if(nRepeatCount > 0){
		debugStr("2262 is busy sending");
		return 0;
	}

	pBit = bitTypeSeq;

	//load synchronize bit
	*pBit = BitSync;pBit++;
	//load address
	for(i=0; i < ADDR_LEN_2262; ++i){
		*pBit = pAddr[i]; pBit++;
	}
	//load data
	for(i=0; i < DATA_LEN_2262; ++i){
		*pBit = pData[i]; pBit++;
	}

	loadFirstBit();

	nRepeatCount = 4;	//send 4 times according to 2262 protocal
	//everything is in position...
	return 1;
}


