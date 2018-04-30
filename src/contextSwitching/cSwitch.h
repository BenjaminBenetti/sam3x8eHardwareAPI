#ifndef C_SWITCH_H_
#define C_SWITCH_H_
#include <stdint.h>

/**
  trigger the pend sv interrupt
*/
void triggerPendSV(void);


#define SAVE_STATE() \
  asm( \
    "push {r4-r11}\n" \
  ) \

#define SAVE_STATE_PSP() \
  asm ( \
    "mrs r0, psp \n"\
    "stmdb r0!, {r4-r11}\n" \
    "msr psp, r0 \n"\
  ) \

#define RESTORE_STATE() \
  asm( \
    "pop {r4-r11}\n" \
  ) \

#define RESTORE_STATE_PSP() \
  asm ( \
    "mrs r0, psp \n"\
    "ldmia r0!, {r4-r11}\n" \
    "msr psp, r0 \n"\
  ) \

#define MSP_TO_PSP(sp) \
  asm( \
    "msr PSP, %[sptr] \n" \
    : \
    : [sptr] "r" (sp) \
    : \
  ) \

#define READ_PSP(dest) asm("mrs %[d], psp\n" : [d] "=r" (dest))
#define READ_MSP(dest) asm("mrs %[d], msp\n" : [d] "=r" (dest))

#define IRQ_RET_PSP() asm("mov r0, 0xFFFFFFFD\n bx r0\n")
#define IRQ_RET_MSP() asm("mov r0, 0xFFFFFFF9\n bx r0\n")

#endif /*C_SWITCH_H_*/
