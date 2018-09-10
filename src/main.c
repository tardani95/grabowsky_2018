/******************************************************************************
File        : main.c
Author      : Daniel Tar
Version     : 0.2
Copyright   :
Description :
Info        : 07-09-2018
******************************************************************************/

/*==========================================================================*/
/*                               Includes								 	*/
/*==========================================================================*/
#include <stddef.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "conf.h"
#include "periph.h"
#include "util.h"
#include "control.h"
#include "nav.h"

/*==========================================================================*/
/*                            Private typedef								*/
/*==========================================================================*/


/*==========================================================================*/
/*                            Private define								*/
/*==========================================================================*/


/*==========================================================================*/
/*                            Private macro									*/
/*==========================================================================*/


/*==========================================================================*/
/*                            Private variables								*/
/*==========================================================================*/


/*==========================================================================*/
/*                      Private function prototypes							*/
/*==========================================================================*/


/*==========================================================================*/
/*                          Private functions								*/
/*==========================================================================*/
#ifdef DEBUG
	#include "stm32f10x_dbgmcu.h"

	void InitDBG(void){
		DBGMCU_Config(DBGMCU_TIM2_STOP, ENABLE); /* this will make TIM2 stop when core is halted during debug */
		DBGMCU_Config(DBGMCU_TIM3_STOP, ENABLE);
		DBGMCU_Config(DBGMCU_STOP, ENABLE);
	}
#endif

/*==========================================================================*/
/*  							Main program								*/
/*==========================================================================*/
int main(void){

	SystemInit();

#ifdef DEBUG
	InitDBG();
#endif

	/* configure SysTick for delay functions */
//	InitSysTick();


//	InitPeriph();
//	InitNav();

	InitSysTick();
	InitRCC();
//	InitGPIO();
	InitEncoder();

	uint16_t counter = 512;
	TIM_SetCounter(TIM1, 16000);

	/*
		uint16_t ledPins[4]={	PIN_D_RS,
									PIN_D_RF,
									PIN_D_LF,
									PIN_D_LS};
		GPIO_TypeDef* ledPorts[4]={PORT_D_RS,
									PORT_D_RF,
									PORT_D_LF,
									PORT_D_LS};
	*/

	/* TODO - Add your application code here */

	/* Infinite loop */
	while (1){
		counter = TIM1->CNT;

	}
}




/*==========================================================================*/
/*                            Assert functions								*/
/*==========================================================================*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/*
 * Minimal __assert_func used by the assert() macro
 * */
/*extern "C" void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}*/

/*
 * Minimal __assert() uses __assert__func()
 * */
/*extern "C" void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}*/

