#include "timer.h"
#include "stm32f4xx.h"
//Set TIM3 as slave timer, generate PWM
//Set TIm2 as master timer, generate single pulse and control TIM3
void TIMER_INIT(u32 arr, u32 psc)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	//TIM3 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  
	TIM_TimeBaseStructure.TIM_Period=arr-1;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	
	//TIM2 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_Period = arr * 10-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	//TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	
	TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
	          
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC2Ref);
	TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);	
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Gated); 

	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM3,0);
		
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);

}
