/**
@file
@brief EEFC (enhanced flash controller) hardware interface 
*/

#ifndef EEFC_H_
#define EEFC_H_
#include <stdint.h>

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
//end EEFC command id list

/**
  execute a raw EEFC command. see EEFC section in sam3x8e manual for command list.
  @param controller
  id of the EEFC that the command is to be run on.
  Sam3x8e has two. 0 and 1 respectivly
  @param cmd
  EEFC command to execute
  @param farg
  argument for the command
  @param resBuff
  if the command does produce a result, it will be stored here, with NO BOUNDS CHECK!
  @return true on success, false on failure
*/
int rawEEFCCommand(uint8_t controller,uint8_t cmd, uint16_t farg, void * resBuff);

/**
  read the flash descriptor form the EEFC controller.
  BUFFER IS TMP TMP TMP !!! TODO FIX
*/
void readFlashDescriptor(void * buffer);


#endif /*EEFC_H_*/
