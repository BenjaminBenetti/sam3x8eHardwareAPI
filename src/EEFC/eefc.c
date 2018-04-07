#include "eefc.h"
#include <sam3xa/include/sam3x8e.h>
#include "../util/types.h"

int rawEEFCCommand(uint8_t controller, uint8_t cmd, uint16_t farg, void * resBuff){
  Efc * efcPtr = NULL;
  switch(controller){
    case 0:
      efcPtr = EFC0;
      break;
    case 1:
      efcPtr = EFC1;
      break;
    default:
      return FALSE;
  }

  //execute command
  efcPtr->EEFC_FCR = EEFC_FCR_FKEY(0x5A) | EEFC_FCR_FARG(farg) | EEFC_FCR_FCMD(cmd);

  //wait for complete
  for(;;){
    if(efcPtr->EEFC_FSR & EEFC_FSR_FRDY){
      return TRUE;//sucess
    }
    else if ((efcPtr->EEFC_FSR & EEFC_FSR_FCMDE) || (efcPtr->EEFC_FSR & EEFC_FSR_FLOCKE)){
      return FALSE; // bad command
    }
  }
}


void readFlashDescriptor(void * buffer){
}
