#include "nav.h"


/*encoder input capture values*/
uint32_t ENC_Left;
uint32_t ENC_Right;

/*encoder channels*/
capture RA;
//capture RB;
capture LA;
//capture LB;

uint16_t timer1_clk;

float d_wheel = 18;/*mm*/
float gear_ratio = 4;/*#cogs_wheel/#cogs_magnet*/

void InitNav(void){
	InitCapture(&RA);
	InitCapture(&LA);
	GetClkFreq();
}

void InitCapture(capture* capStruct){
	capStruct->prev_cap = 0;
	capStruct->new_cap = 0;
	capStruct->diff_cnt = 0;
	capStruct->diff_ms = 0;
	capStruct->d_wheel = 18;/*mm*/
	capStruct->w = 0;/*rad/ms*/
	capStruct->v = 0;/*mm/s*/
	//capStruct->first_cap = 1;
}

void GetClkFreq(void)
{
	RCC_ClocksTypeDef   clkConfStruct;
	RCC_GetClocksFreq(&clkConfStruct);

	timer1_clk = clkConfStruct.PCLK2_Frequency/1000;/*kHz; if APB2 prescaler = 1!!!*/
}

void ENC_SetLeftValue(uint32_t value){
	ENC_Left = value;
}

void ENC_SetRightValue(uint32_t value){
	ENC_Right = value;
}

uint32_t ENC_GetLeftValue(){
	return ENC_Left;
}

uint32_t ENC_GetRightValue(){
	return ENC_Right;
}



float duty_start = 0; //MOT_PWM_MAX_DUTY/2;

uint8_t dir_L = 0, dir_R = 0;
uint8_t go = 0;/*for enabling/disabling control, global variable*/

void EXTI9_5_IRQHandler(void){

	/* interrupt request from encoderL ? */
	if(SET == EXTI_GetITStatus(EXTI_Line8)){
		EXTI->PR = EXTI_Line8;
		ENC_Left++;
		return;
	}

	/* interrupt request from button ? */
	if(SET == EXTI_GetITStatus(EXTI_Line5)){
		EXTI->PR = EXTI_Line5;
		/*do stuff when button pressed*/
		/*start/stop motors*/
		if(!go){
			go = 1;
			MotCtl(duty_start, MOT_R);
			MotCtl(duty_start, MOT_L);
		}
		else {
			MotCtl(0, MOT_R);
			MotCtl(0, MOT_L);
			go = 0;
		}
	}
}


void EXTI15_10_IRQHandler(void){
	/* interrupt request from encoderR ? */
	if(SET == EXTI_GetITStatus(EXTI_Line10)){
		EXTI->PR = EXTI_Line10;
		ENC_Right++;
	}
}

/* not used at the moment */
void TIM1_IRQHandler(void){
	uint16_t timer_sr = TIM1->SR;

	/*if(timer_sr&TIM_SR_CC1IF) //channel1
	{
		RB.prev_cap = RB.new_cap;
		RB.new_cap = TIM1->CCR1;
		if(RB.prev_cap<RB.new_cap)
			RB.diff_cnt = RB.new_cap-RB.prev_cap;
		else
			RB.diff_cnt = (TIM1->ARR - RB.prev_cap) + RB.new_cap;
	}*/


//	if(timer_sr&TIM_SR_CC2IF)/*channel2*/
//	{
//		RA.prev_cap = RA.new_cap;
//		RA.new_cap = TIM1->CCR2;
//		if(RA.prev_cap<RA.new_cap)
//			RA.diff_cnt = RA.new_cap-RA.prev_cap;
//		else
//			RA.diff_cnt = (TIM1->ARR - RA.prev_cap) + RA.new_cap;
//
//		RA.diff_ms = (float)RA.diff_cnt/(timer1_clk*(TIM1->PSC+1));/*ms; IC1PSC = 1, rising edge*/
//		RA.w = 512/(RA.diff_ms*gear_ratio);/*rad/ms*/
//		RA.v = RA.w*RA.d_wheel/2*1000;/*mm/s*/
//	}
//	else if(timer_sr&TIM_SR_CC3IF)/*channel3*/
//	{
//		LA.prev_cap = LA.new_cap;
//		LA.new_cap = TIM1->CCR3;
//		if(LA.prev_cap<LA.new_cap)
//			LA.diff_cnt = LA.new_cap-LA.prev_cap;
//		else
//			LA.diff_cnt = (TIM1->ARR - LA.prev_cap) + LA.new_cap;
//
//		LA.diff_ms = (float)LA.diff_cnt/(timer1_clk*(TIM1->PSC+1));/*IC2PSC = 1, rising edge*/
//		LA.w = 512/(LA.diff_ms*gear_ratio);/*rad/ms*/
//		LA.v = LA.w*LA.d_wheel/2*1000;/*mm/s*/
//	}

	/*if(timer_sr&TIM_SR_CC4IF)//channel4
	{
		LB.prev_cap = LB.new_cap;
		LB.new_cap = TIM1->CCR4;
		if(LB.prev_cap<LB.new_cap)
			LB.diff_cnt = LB.new_cap-LB.prev_cap;
		else
			LB.diff_cnt = (TIM1->ARR - LB.prev_cap) + LB.new_cap;
	}*/
}
