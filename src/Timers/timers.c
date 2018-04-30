#include "timers.h"
#include <sam3xa/include/sam3x8e.h>
#include "../general/general.h"

void initRTT(void){
  RTT->RTT_MR = RTT_MR_RTPRES(0x20);
}

void sleep(int milliseconds) {
	uint32_t sleep_until = RTT->RTT_VR + milliseconds;
	while ((sleep_until - RTT->RTT_VR) > 0);
}

uint32_t getTime(){
  return RTT->RTT_VR;
}

void systickConfig(uint32_t interval){
  SysTick_Config((SYSTEM_CLOCK_SPEED / 1000)*interval);
  stopSysTick();
}

void stopSysTick(void){
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startSysTick(void){
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
