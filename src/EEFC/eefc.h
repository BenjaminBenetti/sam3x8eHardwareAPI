/**
@file
@brief EEFC (enhanced flash controller) hardware interface
*/

#ifndef EEFC_H_
#define EEFC_H_
#include <stdint.h>
#include <stdbool.h>

/**
  this structure describes the layout of flash memory on the board.
*/
struct FlashDescriptor{
  uint32_t flashID;
  uint32_t flashSize;
  uint32_t pageSize;
  uint32_t numberOfPlanes;

  uint32_t * bytesInPlane;//list
  uint32_t lenBytesInPlane;

  uint32_t numberOfLockBits;

  uint32_t * bytesPerLockRegion; // lsit
  uint32_t lenBytesPerLockRegion;

} FlashDescriptor;

/**
  the number of flash controllers on the sam3x8e
*/
#define FLASH_CONTROLLER_COUNT 2

/**
  get the start address for controller
  @param controller the controller for which the start address is to be fetech.
  @return the address at which said flash memory starts
*/
uint8_t * getFlashStartAddress(uint16_t controller);

/**
  read the flash descriptor form the EEFC controller. and populate the flash descriptor structure
  @param fDesc the flash descriptor strcture to be populated by the function call.
  Note, you must have pre allocated, bytesInPlane and bytesPerLockRegion arrays
  @param controller the controller on which you want to read the flash descriptor
  @param lenPlanesMax the max number of plane words that can be placed in bytesInPlane array.
  @param lenLockRegionMax the max number of lock region words that can be placed in bytesPerLockRegion
  @return the flash descriptor structure will be filled out. lenBytesInPlane and lenBytesPerLockRegion
  will be set to the number of actual values in there respective arrays. if lenBytesInPlane or lenBytesPerLockRegion is
  equal to there max counterparts, there may be additional regions / planes on the flash chip.
  @see FlashDescriptor
  @see FLASH_CONTROLLER_COUNT
*/
void readFlashDescriptor(struct FlashDescriptor * fDesc, uint16_t controller, uint32_t lenPlanesMax, uint32_t lenLockRegionMax);

/**
  read the specified page from the specified controller
  @param controller the controller from which to read
  @param page the page which is to be read
  @param buffer in to which the page is read. MUST be large enough to hold a page of data
  @return true on success.
  @see readFlashDescriptor
*/
bool readPage(uint16_t controller, uint16_t page, uint8_t * buffer);

/**
  write a page to the specified controller at the specified page address
  @param controller the controller to which you whish to write
  @param page the page you want to write
  @param buffer a buffer containing the data to be writen to the page. MUST be as large as a page
  @return true on success
  @see readFlashDescriptor
*/
bool writePage(uint16_t controller, uint16_t page, uint8_t * buffer);

#endif /*EEFC_H_*/
