#ifndef _MY_ENCODER_H_
#define _MY_ENCODER_H_

#include "config.h"
#include "STC15FXXXX.h"
#include "encoder_bit.h"

//function prototype of loading bit
typedef void (*PLoadFunc)();


//there are totally 4 type of bits in 2262 coding rule
#define BIT_TYPE_COUNT	4
enum EBitType {BitSync=0, BitF, Bit0, Bit1};

//totally 13 bit to form a 2262 word
#define ADDR_LEN_2262	8
#define DATA_LEN_2262	4
#define WORD_LEN_2262	ADDR_LEN_2262+DATA_LEN_2262+1

extern u8 nRepeatCount;	//indicate which turn of sending
extern u8 idxBitSending;	//indicate which bit is sending
extern u8 bitTypeSeq[WORD_LEN_2262];	//bit stock for sending

extern PLoadFunc bitTypeFunc[BIT_TYPE_COUNT];

void loadFirstBit();
void send2262(u8 p1, u8 p2);

#endif

