#include "eefc.h"
#include <sam3xa/include/sam3x8e.h>
#include <string.h>

//EEFC command id list
#define EEFC_GETD 0x00 // get descriptor
#define EEFC_WP   0x01 // write page
#define EEFC_WPL  0x02 // write page & lock
#define EEFC_EWP  0x03 // erase page & write
#define EEFC_EWPL 0x04 // erase page & write & lock
#define EEFC_EA   0x05 // danger Batman! (will erase all flash!)
#define EEFC_SLB  0x08 // lock page
#define EEFC_CLB  0x09 // unlock page
#define EEFC_GLB  0x0A // check if page locked
#define EEFC_SGPB 0x0B
#define EEFC_CGPB 0x0C
#define EEFC_GGPB 0x0D
#define EEFC_STUI 0x0E
#define EEFC_SPUI 0x0F
#define EEFC_GCALB 0x10

//return pointer to EEFC chip based on con
Efc * getEEFC(uint16_t con){
  //only two chips on sam3x8e
  Efc * Eptr = NULL;
  switch(con){
    case 0:
      Eptr = EFC0;
      break;
    case 1:
      Eptr = EFC1;
      break;
  }
  return Eptr;
}

static int rawEEFCCommand(uint8_t controller, uint8_t cmd, uint16_t farg){
  Efc * efcPtr = getEEFC(controller);

  //execute command
  efcPtr->EEFC_FCR = EEFC_FCR_FKEY(0x5A) | EEFC_FCR_FARG(farg) | EEFC_FCR_FCMD(cmd);

  //wait for complete
  for(;;){
    if(efcPtr->EEFC_FSR & EEFC_FSR_FRDY){
      return true;//sucess
    }
    else if ((efcPtr->EEFC_FSR & EEFC_FSR_FCMDE) || (efcPtr->EEFC_FSR & EEFC_FSR_FLOCKE)){
      return false; // bad command
    }
  }
}


void readFlashDescriptor(struct FlashDescriptor * fDesc, uint16_t controller, uint32_t lenPlanesMax, uint32_t lenLockRegionMax){
  Efc * efcPtr = getEEFC(controller);

  rawEEFCCommand(controller,EEFC_GETD, 0x0);

  fDesc->flashID        = efcPtr->EEFC_FRR;
  fDesc->flashSize      = efcPtr->EEFC_FRR;
  fDesc->pageSize       = efcPtr->EEFC_FRR;
  fDesc->numberOfPlanes = efcPtr->EEFC_FRR;

  int i = 0;
  while(i < fDesc->numberOfPlanes){
    if(i < lenPlanesMax){
      *(fDesc->bytesInPlane + i)  = efcPtr->EEFC_FRR;
    }
    else{
      //read reg to "advance" value
      uint32_t read = efcPtr->EEFC_FRR;
    }

    i++;
  }
  fDesc->lenBytesInPlane = i;

  fDesc->numberOfLockBits = efcPtr->EEFC_FRR;

  i = 0;
  while(i < fDesc->numberOfLockBits){
    if(i < lenLockRegionMax){
      *(fDesc->bytesPerLockRegion + i) = efcPtr->EEFC_FRR;
    }
    else {
      //read reg to "advance" value
      uint32_t read = efcPtr->EEFC_FRR;
    }
    i++;
  }
  fDesc->lenBytesPerLockRegion = i;
}

uint8_t * getFlashStartAddress(uint16_t controller){
  //return flash address as described on page 31 of sam3x8e datasheet
  switch(controller){
    case 0:
      return (void *) 0x00080000;
    case 1:
      return (void *) 0x000C0000;
  }
}

bool readPage(uint16_t controller, uint16_t page, uint8_t * buffer){
  struct FlashDescriptor fd;
  readFlashDescriptor(&fd,controller,0,0);

  uint8_t * pagePtr = getFlashStartAddress(controller) + fd.pageSize*page;

  memcpy(buffer,pagePtr,fd.pageSize);
  return true;
}

bool readPageAuto(uint16_t page, uint8_t * buffer){
  struct FlashDescriptor fd;
  readFlashDescriptor(&fd,0,0,0);
  if(page > (fd.flashSize / fd.pageSize)){
    return readPage(1,page - (fd.flashSize / fd.pageSize) - 1,buffer);
  }
  else {
    return readPage(0,page,buffer);
  }

}

bool writePage(uint16_t controller, uint16_t page, uint8_t * buffer){
  struct FlashDescriptor fd;
  readFlashDescriptor(&fd,controller,0,0);

  uint32_t * pagePtr = (uint32_t*)getFlashStartAddress(controller) + fd.pageSize*page;

  //cpy in to EEFC latch buffer
  //note! memcpy not used here as flash MUST BE WRITEN IN WORD (32bit) CHUNKS!!!!!
  uint32_t * bptr32 = (uint32_t*)buffer;
  for(int i =0; i < FLASH_PAGE_SIZE/4;i ++){
    *pagePtr = *bptr32;
    pagePtr = pagePtr + 1;
    bptr32 = bptr32 + 1;
  }

  //send write command
  return rawEEFCCommand(controller,EEFC_EWP,page);
}

bool writePageAuto(uint16_t page, uint8_t * buffer){
  struct FlashDescriptor fd;
  readFlashDescriptor(&fd,0,0,0);
  if(page > (fd.flashSize / fd.pageSize)){
    return writePage(1,page - (fd.flashSize / fd.pageSize) - 1,buffer);
  }
  else {
    return writePage(0,page,buffer);
  }
}
