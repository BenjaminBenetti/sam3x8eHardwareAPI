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

#define NANO_PER_CLOCK (1000000000 / SYSTEM_CLOCK_SPEED)
#define NANO_OVERHEAD (8 * NANO_PER_CLOCK)
__attribute__ ((naked)) void nanoSleep(uint32_t nanosec){
  asm(// assume about 8 instruction overhead on entry
    "mov r0, %[over] \n"
    "nanoSleepStart: \n"
    "add r0, %[clk] \n"
    "cmp r0, %[nano] \n"
    "blo nanoSleepStart \n"
    "bx lr \n"
    :
    : [nano] "r" (nanosec), [clk] "r" (NANO_PER_CLOCK*3), [over] "r" (NANO_OVERHEAD)
    :
  );
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
