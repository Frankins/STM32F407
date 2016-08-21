#include "timer.h"
#include "stm32f4xx.h"


void TIMER_INIT(u32 arr, u32 psc,u16 pulse)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//TIM2 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  
	TIM_TimeBaseStructure.TIM_Period=arr-1;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  

	//TIM1 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_Period = arr * pulse *2 -1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);
        
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Gated);

	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM1,0);
	TIM_SetCounter(TIM2,0);

	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);

}
