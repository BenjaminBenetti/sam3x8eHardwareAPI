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
sleep for the given time in nanoseconds.
note with 84Mhz clock this sleep will be +/- 11 ns.
you can not wait less than about 143 ns as this is the call overhead (depending on pipelining ofc ).
*/
void nanoSleep(uint32_t nanosec);

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
