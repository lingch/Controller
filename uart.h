#ifndef __UART_H
#define __UART_H

//#include	"config.h"
#include	"STC15Fxxxx.H"

void UartInit();
void SendData(unsigned char ch);
void SendString(char *s);



#endif