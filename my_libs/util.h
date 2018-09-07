#ifndef _UTIL_H

#define _UTIL_H


#include "conf.h"

#include "stm32f10x.h"


extern __IO uint32_t SysTickCnt; /* make this available in all files that include periph */


void DelayMs(uint32_t t);








#endif
