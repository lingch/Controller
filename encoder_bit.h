#ifndef _MY_ENCODER_BIT_H_
#define _MY_ENCODER_BIT_H_

#include "STC15FXXXX.h"
#include "config.h"

u8* loadBit(u8 *pBit, u8 bitPattern);

//convenience function to loadBit
u8* loadBit0(u8 *pBit);
u8* loadBit1(u8 *pBit);
u8* loadBitF(u8 *pBit);
u8* loadBitSync(u8 *pBit);

#endif

