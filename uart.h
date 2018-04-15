#ifndef __UART_H
#define __UART_H

//#include	"config.h"
#include	"STC15Fxxxx.H"

#define UartInit UartInitUseT2

void UartInit();
void SendData(unsigned char ch);
void SendString(char *s);



#endif