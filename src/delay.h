#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x_conf.h"

static __IO uint32_t sysTickCnt;

void SysTick_Init();

void TimerDecrement();

void delay_ms(uint32_t ms);

void delay_ns(uint32_t ns);


#endif /* DELAY_H_ */
