#include "timer.h"
#include "stm32f4xx.h"
//Set TIM3 as slave timer, generate PWM
//Set TIM5 as master timer, generate single pulse and control TIM3

void TIMER_INIT(u32 arr, u32 psc,u16 pulse)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	    //开启定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//开启IO口时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 		//复用PA7引脚到TIM3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        	//不上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	//TIM3 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  				//预分频系数
	TIM_TimeBaseStructure.TIM_Period=arr-1;  					//自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			     //PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		 //输出极性高
	TIM_OCInitStructure.TIM_Pulse = arr/2-1;						 //占空比50%
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  				 //使能预装载寄存器
	
	//TIM5 Configuration
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;				//预分频系数
	TIM_TimeBaseStructure.TIM_Period = arr * pulse -1;			//自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
 
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			//PWM模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //TIM_OCPolarity_High; 
	//TIM_OCInitStructure.TIM_Pulse = arr-1;
	TIM_OC2Init(TIM5, &TIM_OCInitStructure); 	
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	
	TIM_SelectOnePulseMode(TIM5, TIM_OPMode_Single);			//设TIM5为单脉冲模式
	          
	TIM_SelectOutputTrigger(TIM5, TIM_TRGOSource_OC2Ref);		//TIM5触发
	TIM_SelectInputTrigger(TIM3, TIM_TS_ITR2);					//使能TIM3触发功能，通道ITR2
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Gated); 				//TIM3为从定时器

	TIM_SelectMasterSlaveMode(TIM5,TIM_MasterSlaveMode_Enable); //TIM5为主定时器

	TIM_SetCounter(TIM5,0);										//计数器清零
	TIM_SetCounter(TIM3,0);										//计数器清零
		
	TIM_Cmd(TIM5,ENABLE);										//计数器开始计数
	TIM_Cmd(TIM3,ENABLE);										//计数器开始计数

}