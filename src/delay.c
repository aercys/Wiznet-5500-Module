#include "delay.h"

void SysTick_Handler(void) {
	TimerDecrement();
}

void SysTick_Init() {
	while(SysTick_Config(SystemCoreClock / 1000000) != 0);
}

void TimerDecrement() {
	if(sysTickCnt != 0x00)
		sysTickCnt--;
}

void delay_ms(uint32_t ms) {
	while(ms--) {
		sysTickCnt = 1000;
		while(sysTickCnt != 0x00);
	}
}

void delay_ns(uint32_t ns) {
	sysTickCnt = ns;
	while(sysTickCnt != 0x00);
}
