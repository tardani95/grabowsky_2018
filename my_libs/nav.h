#ifndef _NAV_H

#define _NAV_H

#include "periph.h"


void TIM1_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void I2C2_EV_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

void GetClkFreq(void);

typedef struct{
	uint16_t prev_cap;
	uint16_t new_cap;
	uint16_t diff_cnt;
	float d_wheel;/*diameter of wheel in mm*/
	float diff_ms;
	float w;/*angular velocity of wheel in rad/ms*/
	float v;/*tangential velocity of wheel in mm/s*/
	//uint8_t first_cap;
}capture;

void InitNav(void);
void InitCapture(capture* capStruct);

void ENC_SetLeftValue(uint32_t value);
void ENC_SetRightValue(uint32_t value);

uint32_t ENC_GetLeftValue();
uint32_t ENC_GetRightValue();

#endif
