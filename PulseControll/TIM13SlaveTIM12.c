#include "timer.h"
#include "stm32f4xx.h"
//Set TIM12 as slave timer, generate PWM
//Set TIm13 as master timer, generate single pulse and control TIM3
void TIMER_INIT(u32 arr, u32 psc,u16 pulse)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	//TIM9 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  
	TIM_TimeBaseStructure.TIM_Period=arr-1;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  
	
	//TIM13 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_Period = arr * pulse-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	//TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC1Init(TIM13, &TIM_OCInitStructure); 	
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable); 
	
	TIM_SelectOnePulseMode(TIM13, TIM_OPMode_Single);
	          
	TIM_SelectOutputTrigger(TIM13, TIM_TRGOSource_OC1Ref);
	TIM_SelectInputTrigger(TIM12, TIM_TS_ITR2);	
	TIM_SelectSlaveMode(TIM12,TIM_SlaveMode_Gated); 

	TIM_SelectMasterSlaveMode(TIM13,TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM13,0);
	TIM_SetCounter(TIM12,0);
		
	TIM_Cmd(TIM13,ENABLE);
	TIM_Cmd(TIM12,ENABLE);

}
