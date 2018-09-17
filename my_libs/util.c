/*
******************************************************************************
File        : util.c
Author      : Daniel Tar
Version     :
Copyright   :
Description :
Info        : 07-09-2018
*******************************************************************************
*/
#include "util.h"
#include "periph.h"

/******************************************************************************/
/*                          Private variables                                 */
/******************************************************************************/
__IO uint32_t SysTickCount=0;	/* counts nr. of ticks since boot */
__IO uint8_t hbState=0;		 	/* heartbeat */


/******************************************************************************/
/*                          Function definitions                              */
/******************************************************************************/

/**
  * @brief  increment SysTick counter, useful for delay functions
  *         with systick f=1kHz, it overflows every 1.19*1000 hours
  * @param  None
  * @retval None
  */
void Init_SysTick(void){

	RCC_ClocksTypeDef RCC_ClockStruct;
	RCC_GetClocksFreq(&RCC_ClockStruct);

	SysTick_Config(RCC_ClockStruct.SYSCLK_Frequency/SYS_TICK_FREQ-1); /* 1ms/SysClk tick */

}

/**
  * @brief  increment SysTick counter, useful for delay functions
  *         with systick f=1kHz, it overflows every 49 days
  * @param  None
  * @retval None
  */

void SysTick_Handler(void){
	SysTickCount++;

	if(SysTickCount%2000==0){
			if(hbState){
				GPIO_SetBits(LEDs_Port, LED1_Pin);
			}
			else
				GPIO_ResetBits(LEDs_Port, LED1_Pin);

			hbState=!hbState;
	}
}

uint32_t GetSysTickCount(void){
	return SysTickCount;
}

uint32_t Millis(void){
	return SysTickCount/1000;
}

/**
  * @brief  This function freeze the cpu for x [us]
  * @param  time in [us]
  * @retval None
  */
void DelayUs(uint32_t us){
	uint32_t startTick=SysTickCount;

	/*overflow check*/
	if(UINT32_MAX - startTick <= us ){
		SysTickCount = startTick = 0;
	}

	while(SysTickCount-startTick <= us){}
}

/**
  * @brief  This function freeze the cpu for x [ms]
  * @param  time in [ms]
  * @retval None
  */
void DelayMs(uint32_t ms){
	for(uint32_t i = 0; i<ms; ++i){
		DelayUs(1000); /*  the error grows with higher ms values*/
	}
}

/**
  * @brief  This function freeze the cpu for x [sec]
  * @param  time in [sec]
  * @retval None
  */
void DelaySec(uint32_t sec){
	for(uint32_t i = 0; i<sec; ++i){
		DelayMs(999); /*  the error grows with higher sec values*/
	}
}


