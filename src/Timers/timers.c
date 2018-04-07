#include "timers.h"
#include <sam3xa/include/sam3x8e.h>

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
