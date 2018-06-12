/**
@file
@brief general / mics hardware functions
*/
#ifndef GENERAL_HW_H_
#define GENERAL_HW_H_
#include "stdint.h"

/**
  initialize the sam3x8e main clock. this clock drives the entire system!
  i.e. all peripherals and the main core clock. many features may not work
  correctly if the main clock is not running at 84Mhz. call this near the start
  of the program.
*/
void setupMainClock(void);

// start random number generator
void startTRNG(void);

//get random 32 bit number from TRNG
uint32_t random(void);

#define SYSTEM_CLOCK_SPEED 84000000 // 84 Mhz

#endif
