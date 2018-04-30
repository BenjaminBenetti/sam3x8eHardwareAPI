/**
@file
@brief hardware timer interface
*/

#ifndef TIMERS_H_
#define TIMERS_H_
#include <stdint.h>

/**
  start the RTT (required for some time related functions)
*/
void initRTT(void);

/**
sleep for the given number of milliseconds
@param milliseconds the number of milliseconds to sleep for.
@see initRTT must be called first
*/
void sleep(int milliseconds);

/**
@return the current time in milliseconds since power on. this value will roll over
every 1,700 ish hours.
*/
uint32_t getTime();

/**
  start initialize systick to interval in ms
  @see startSysTick
  @see stopSysTick
*/
void systickConfig(uint32_t interval);

/**
  stops system tick and resets in to TOP
*/
void stopSysTick(void);

/**
  start the systick timer at TOP
*/
void startSysTick(void);

#endif
