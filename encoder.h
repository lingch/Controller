#ifndef _MY_ENCODER_H_
#define _MY_ENCODER_H_

#include "config.h"
#include "STC15FXXXX.h"
#include "encoder_bit.h"

//there are totally 4 type of bits in 2262 coding rule
#define BIT_TYPE_COUNT	4

void initEncoder();
void send2262();
u8* loadWord(u8 *pAddr, u8 *pData);

#endif

