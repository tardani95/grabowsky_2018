#include "util.h"



void DelayMs(uint32_t t){
	uint32_t startTick=SysTickCnt;

	while(SysTickCnt-startTick<=t*10){}
}



