#include <stdarg.h>
#include <stdio.h>

#include "uart.h"

#define DEBUG_BUF_LEN	100

char buf[DEBUG_BUF_LEN];

void debugInit(){
	//buf = (char*)malloc(DEBUG_BUF_LEN);
}

void debug(char *fmt, ...){
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf(buf,fmt, argptr);
	va_end(argptr);
	 
	SendString(buf);
}

void debugStr(char *str){
	debug("%s\r\n",str);
}


