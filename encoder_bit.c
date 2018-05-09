#include "debug.h"
#include "encoder_bit.h"

//wave pattern definition
#define BIT0 = 0x88;
#define BIT1 = 0xee;
#define BITF = 0x8e;

u8 BITD[3] = {BIT0,BIT1,BITF};
u8 BITSYNC[4] = {0x80,0x00,0x00,0x00};	

u8* loadBit(u8 *pBit, u8 bitPattern){
	int i;

	for(i=0;i<8;++i){
		*pBit = (bitPattern & (0x01 << i)) >> i;
		pBit++;
	}

	return pBit;
}

u8* loadBit0(u8 *pBit){
	return loadBit(pBit, BIT0);
}

u8* loadBit1(u8 *pBit){
	return loadBit(pBit, BIT1);
}

u8* loadBitF(u8 *pBit){
	return loadBit(pBit, BITF);
}

u8* loadBitSync(u8 *pBit){
	pBit = loadBit(pBit, BITSYNC[0]);
	pBit = loadBit(pBit, BITSYNC[1]);
	pBit = loadBit(pBit, BITSYNC[2]);
	pBit = loadBit(pBit, BITSYNC[3]);

	return pBit;
}

