#ifndef D_IO_H_
#define D_IO_H_
#include <stdint.h>
#include <stdbool.h>
#include <sam3xa/include/sam3x8e.h>

/********************
 ---- PIO banks ----*/
#define PIO_BANK_A 11
#define PIO_BANK_B 12
#define PIO_BANK_C 13
#define PIO_BANK_D 14
/********************/

// -- PIO direction --
#define PIO_IN 0
#define PIO_OUT 1


/**
  enable the specified pio bank. see pinout of board for settings
*/
void enableIOBank(uint8_t bank);

/**
  @see enableIOBank
*/
void enableAllIOBanks();

/**
  disable specified pio bank
*/
void disableIOBank(uint8_t bank);


/**
  enable the specified pin. i.e. set it to digital output (disable peripheral control)
  @param io bank the pin belongs to.
  @param pin the pin to enable. names are like "PIO_PB27" (pin B 27).
  @param direction the direction of the pin (PIO_IN or PIO_OUT)
*/
void enablePin(uint8_t bank, uint32_t pin, uint8_t direction);

/**
  disable pin.
  @see enablePin
*/
void disablePin(uint8_t bank, uint32_t pin);

/**
  set direction of pin.
  @param bank the bank that the pin belongs to. A,B,C,D
  @param pin the pin to modify
  @param direction new direction setting for the pin IN/OUT
*/
void setPinDirection(uint8_t bank, uint32_t pin, uint8_t direction);

/**
  read signal on pin.
  @retrun 0 or 1
*/
uint8_t readPin(uint8_t bank, uint32_t pin);

/**
  set logic level on pin
  @param bank io bank
  @param pin pin to set
  @param llvl logic level to set on pin. 1 or 0
*/
void setPin(uint8_t bank, uint32_t pin, uint8_t llvl);

/**
  enable or disable the pullup on the given pin
*/
void setPullup(uint8_t bank, uint32_t pin, bool en);


#endif /*D_IO_H_*/
