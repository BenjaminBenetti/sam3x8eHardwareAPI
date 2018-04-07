#ifndef TIMERS_H_
#define TIMERS_H_
#include <stdint.h>

void initRTT(void);

void sleep(int milliseconds);

uint32_t getTime();

#endif
