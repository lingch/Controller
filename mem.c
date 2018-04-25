#include "mem.h"

#define HEAP_SIZE	2048
static u8 mem[HEAP_SIZE];

void memInit(){
	init_mempool(mem,HEAP_SIZE);
}

