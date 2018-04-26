#ifndef _MGR_STATE_H_
#define _MGR_STATE_H_

#include "config.h"

#include	"STC15Fxxxx.H"

enum EState {Idle=0, Hold1, Hold2, Hold3};

void mgrStateInit();

void detectKeyRelease();
void rotateState();
enum EState getCurState();


#endif

