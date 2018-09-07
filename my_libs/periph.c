#include "periph.h"


/* init every peripheral used,  */
void InitPeriph(void){
	InitRCC();
	InitSysTick();

	InitGPIO();

	InitEXTI();

	InitADC();
	InitTIM();

	//InitUSART();
	//InitNVIC();
	//InitI2C();
#ifdef _DEBUG
	InitDBG();
#endif
}


void InitRCC(void){

	RCC_DeInit();
	RCC_ClockSecuritySystemCmd(DISABLE);

	/* disable HSE */
	RCC_HSEConfig(RCC_HSE_OFF);

	/* enable HSI, 8 Mhz */
	RCC_HSICmd(ENABLE);

	/* Wait till HSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	RCC_PLLCmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	FLASH_SetLatency(FLASH_Latency_1);
	FLASH_Unlock();
	/* Set PLLCLK as sys clock*/
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); /* 64 MHz */



	/* Set HCLK, PCLK1, and PCLK2 */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);	/* 64 MHz */
	RCC_PCLK1Config(RCC_HCLK_Div2); 	/* 32 MHz */
	RCC_PCLK2Config(RCC_HCLK_Div1); 	/* 64 MHz */

	/* Set ADC clk */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); /* 10.6 MHz */

#ifdef _DEBUG_MCO
	RCC_MCOConfig(RCC_MCO_SYSCLK); /*output SYSCLK on PA8, GPIO_Pin_PA8 must be AFIO */
#endif
}



void InitGPIO(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); /* frees up PB3 and PA15 */
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); /* TIM2_CH1=PA15, TIM2_CH2=PB3 */

	GPIO_InitTypeDef GPIOInitStruct;
	GPIO_StructInit(&GPIOInitStruct);

	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* GPIOA, outputs */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Pin= PIN_D_RS | PIN_D_RF;

	GPIO_Init(GPIOA, &GPIOInitStruct);

	/* GPIOA, inputs */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Pin= PIN_ENC_IDX_R | PIN_ENC_LSB_R | PIN_ENC_DIR_R | PIN_ENC_LSB_L | PIN_ENC_DIR_L;

	GPIO_Init(GPIOA, &GPIOInitStruct);

	/* GPIOA, analog in */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIOInitStruct.GPIO_Pin= PIN_ADC1_IN0 | PIN_ADC1_IN1 | PIN_ADC1_IN3 | PIN_ADC1_IN4;

	GPIO_Init(GPIOA, &GPIOInitStruct);

	/* GPIOA, afio */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Pin =PIN_DRV_AE;

	GPIO_Init(GPIOA, &GPIOInitStruct);




	/**************/

	/* GPIOB, outputs */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Pin= PIN_D_LS | PIN_D_LF | PIN_LED0 | PIN_LED1 | PIN_DRV_AP | PIN_DRV_BP;

	GPIO_Init(GPIOB, &GPIOInitStruct);

	/* GPIOB, inputs */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Pin= PIN_SW0 | PIN_ENC_IDX_L;

	GPIO_Init(GPIOB, &GPIOInitStruct);

	/* GPIOB, afio */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Pin =PIN_DRV_BE;

	GPIO_Init(GPIOB, &GPIOInitStruct);


#ifdef _DEBUG_MCO
	/* MCO ouput. See init RCC for further options */
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Pin =GPIO_Pin_8;

	GPIO_Init(GPIOA, &GPIOInitStruct);
#endif

}


void InitEXTI(void){

	EXTI_InitTypeDef EXTI_InitStructure;

	/*button0*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);

	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*encoderL DIR*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*encoderR DIR*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);

	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*nvic*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitTypeDef nvicStructure;

	/*button0+encL_DIR nvic*/
	nvicStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvicStructure);

	/*encR_DIR nvic*/
	nvicStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvicStructure);
}

void InitTIM(void){
	InitTIM1();
	InitTIM2();
	InitTIM4();
}



void InitADC(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;


	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adcBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	//DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SAMPLE_TIME);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SAMPLE_TIME);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 3, ADC_SAMPLE_TIME);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SAMPLE_TIME);

	ADC_DiscModeChannelCountConfig(ADC1,1);

	ADC_DiscModeCmd(ADC1,ENABLE);
	/* Enable ADC1 DMA */
	//ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1,ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
}




uint16_t ledPins[4]={	PIN_D_RS,
							PIN_D_LF,
							PIN_D_RF,
							PIN_D_LS};

GPIO_TypeDef* ledPorts[4]={PORT_D_RS,
					PORT_D_LF,
					PORT_D_RF,
					PORT_D_LS};

uint16_t adcBuf[4]={0};

void readADC(void){

	for(uint8_t i=0;i<4;i++){

		GPIO_SetBits(ledPorts[i], ledPins[i]);

		ADC_SoftwareStartConvCmd(ADC1, ENABLE);

		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);   /* Wait until conversion completion */

		GPIO_ResetBits(ledPorts[i], ledPins[i]);


		adcBuf[i]=ADC1->DR;


	}
}




void InitDBG(void){


}

/*Input Capture for encoders*/
void InitTIM1(void){
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Prescaler =0 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 64000-1; /* 1kHz */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3 | TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter= 0x03;

	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	/*TIM1 counter enable*/
	TIM_Cmd(TIM1, ENABLE);

	/*configuring interrupt*/
	TIM_ITConfig(TIM2,(TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4),ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);


}

void InitTIM2(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	TIM_TimeBaseStructure.TIM_Prescaler =0 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = MOT_PWM_T-1; /* 10kHz */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	/* TIM2 counter enable */
	TIM_Cmd(TIM2, ENABLE);

	/* TIM2 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}



/* timer for control loop */
void InitTIM4(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler =0 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 64000-1; /* 1kHz */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);



	NVIC_InitTypeDef nvicStructure;

	nvicStructure.NVIC_IRQChannel = TIM4_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvicStructure);

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

	/* TIM4 counter enable */
	TIM_Cmd(TIM4, ENABLE);
}




/* eg.: MotCtl(0.12, MOT_FRW, MOT_L) */
void MotCtl(float duty,uint8_t side){
	if(duty<MOT_PWM_MAX_DUTY && duty>-MOT_PWM_MAX_DUTY && go){
		// invalid. assert?
		uint16_t newC=(float)MOT_PWM_T*fabsf(duty);

		BitAction phase = MOT_FRW;

		if(duty<0)
			phase=MOT_REV;
		else
			phase=MOT_FRW;


		if(side==MOT_L){
			TIM_SetCompare1(TIM2,newC);
			GPIO_WriteBit(PORT_DRV_AP,PIN_DRV_AP,phase);
		}
		else if(side==MOT_R){
			TIM_SetCompare2(TIM2,newC);
			GPIO_WriteBit(PORT_DRV_BP,PIN_DRV_BP,phase);
		}
		else{
			//invalid. assert?
		}
	}
}



