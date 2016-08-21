#include "timer.h"
#include "stm32f4xx.h"
//Set TIM9 as slave timer, generate PWM
//Set TIm10 as master timer, generate single pulse and control TIM3
void TIMER_INIT(u32 arr, u32 psc,u16 pulse)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOE,&GPIO_InitStructure); 
	
	//TIM9 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  
	TIM_TimeBaseStructure.TIM_Period=arr-1;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  
	
	//TIM10 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_Period = arr * pulse-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	//TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC1Init(TIM10, &TIM_OCInitStructure); 	
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable); 
	
	TIM_SelectOnePulseMode(TIM10, TIM_OPMode_Single);
	          
	TIM_SelectOutputTrigger(TIM10, TIM_TRGOSource_OC1Ref);
	TIM_SelectInputTrigger(TIM9, TIM_TS_ITR2);	
	TIM_SelectSlaveMode(TIM9,TIM_SlaveMode_Gated); 

	TIM_SelectMasterSlaveMode(TIM10,TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM10,0);
	TIM_SetCounter(TIM9,0);
		
	TIM_Cmd(TIM10,ENABLE);
	TIM_Cmd(TIM9,ENABLE);

}
