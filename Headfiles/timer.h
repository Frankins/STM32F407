#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f4xx.h"
#define BACK 0
#define FOR 1
void TIMER_INIT(u32 arr, u32 psc,u16 pulse,u8 dir);
void TIM3_INIT(u32 arr, u32 psc,u16 pulse);

#endif
