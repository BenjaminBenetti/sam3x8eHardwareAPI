#include "cSwitch.h"
#include <sam3xa/include/sam3x8e.h>

void triggerPendSV(void){
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}
