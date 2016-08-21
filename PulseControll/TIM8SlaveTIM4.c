#include "timer.h"
#include "stm32f4xx.h"
//Set TIM4 as slave timer, generate PWM
//Set TIm8 as master timer, generate single pulse and control TIM2

void TIMER_INIT(u32 arr, u32 psc,u16 pulse)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	//TIM4 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  
	TIM_TimeBaseStructure.TIM_Period=arr-1;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  

	//TIM8 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_Period = arr * pulse * 2-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 
	TIM_CtrlPWMOutputs(TIM8,ENABLE);

	TIM_SelectOnePulseMode(TIM8, TIM_OPMode_Single);
        
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_OC1Ref);
	TIM_SelectInputTrigger(TIM4, TIM_TS_ITR3);
	TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_Gated);

	TIM_SelectMasterSlaveMode(TIM8,TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM8,0);
	TIM_SetCounter(TIM4,0);

	TIM_Cmd(TIM8,ENABLE);
	TIM_Cmd(TIM4,ENABLE);

}
