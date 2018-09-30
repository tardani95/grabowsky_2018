#include "control.h"


/* control loop @1kHz */


float T=0.001;

float Kp=1.5; /* P */
float Ki=0.05; /* I */
float Kd=0.1; /* D */

float e=0;
float e_d=0;

float e_max=2000.0;

float I=0;
float D=0;

float Imax=1.0; /* anti wind-up */


float v_max=0.0;

float v_base=0.0; //0.4

#define N_AVG 10

float s=0;

float v=0;

/* control loop @1kHz */
void TIM4_IRQHandler(void){

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM4,  TIM_IT_Update);

        /*switch(controll_mode){

        case MOVE_FWD :{

        }break;

        case TURN :{

        }break;

        case MOVE_X_CELL :{

        }break;

        }*/

//        LEDs_Port->BSRR |= LED0_Pin;
		MPU6050_DMAGetRawAccelGyro();
		readADC();
//		MPU6050_CalcAccelRot();
//		LEDs_Port->BRR |= LED0_Pin;

        /*test pid*/


        e_d=e;

        e=(adcBuf[1]-adcBuf[2])/e_max; // L-R

        D=Kd*(e-e_d)/T;

        I+=Ki*e;

        if(I>Imax)
        	I=Imax;
        if(I<-Imax)
        	I=-Imax;

        I=0;
        s=Kp*e+I+D;


        v=v_max*s/3.5;


		MotCtl(v_base+v,MOT_L);
		MotCtl(v_base-v,MOT_R);


   }
}
