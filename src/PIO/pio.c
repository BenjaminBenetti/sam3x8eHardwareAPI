#include "pio.h"


#define REG_PIO_PER 0
#define REG_PIO_PDR 1
#define REG_PIO_PSR 2
#define REG_PIO_OER 4
#define REG_PIO_ODR 5
#define REG_PIO_OSR 6
//...
#define REG_PIO_SODR 12
#define REG_PIO_CODR 13
//...
#define REG_PIO_PDSR 15
//...
#define REG_PIO_PUDR 24
#define REG_PIO_PUER 25
//...
#define REG_PIO_OWER 40
#define REG_PIO_OWDR 41
static uint32_t * lookupRegister(uint8_t bank, uint8_t reg){
  switch(bank){
    case PIO_BANK_A:
      return (uint32_t *) (&REG_PIOA_PER + reg);

    case PIO_BANK_B:
      return (uint32_t *) (&REG_PIOB_PER + reg);

    case PIO_BANK_C:
      return (uint32_t *) (&REG_PIOC_PER + reg);

    case PIO_BANK_D:
      return (uint32_t *) (&REG_PIOD_PER + reg);

    default:
      return 0;
  }
}


void enableIOBank(uint8_t bank){
  if(bank >= PIO_BANK_A && bank <= PIO_BANK_D){
    REG_PMC_PCER0 |= (1 << bank);
  }
}

void enableAllIOBanks(){
  enableIOBank(PIO_BANK_A);
  enableIOBank(PIO_BANK_B);
  enableIOBank(PIO_BANK_C);
  enableIOBank(PIO_BANK_D);
}


void disableIOBank(uint8_t bank){
  if(bank >= PIO_BANK_A && bank <= PIO_BANK_D){
    REG_PMC_PCER0 &= ~(1 << bank);
  }
}


void enablePin(uint8_t bank, uint32_t pin, uint8_t direction){
  uint32_t * enReg = lookupRegister(bank, REG_PIO_PER);

  *enReg |= pin;
  setPinDirection(bank,pin,direction);
}

void disablePin(uint8_t bank, uint32_t pin){
  uint32_t * dReg = lookupRegister(bank, REG_PIO_PDR);
  *dReg |= pin;
}

void setPinDirection(uint8_t bank, uint32_t pin, uint8_t direction){
  if(direction == PIO_IN){
    uint32_t * outputDisableReg = lookupRegister(bank, REG_PIO_ODR);
    uint32_t * outputWriteDisReg = lookupRegister(bank, REG_PIO_OWDR);
    *outputDisableReg |= pin;
    *outputWriteDisReg |= pin;
  }
  else {
    uint32_t * outputEnableReg = lookupRegister(bank, REG_PIO_OER);
    uint32_t * outputWriteReg = lookupRegister(bank, REG_PIO_OWER);
    *outputEnableReg |= pin;
    *outputWriteReg |= pin;
    setPullup(bank,pin,false);
  }
}

inline uint8_t readPin(uint8_t bank, uint32_t pin){
  uint32_t * pinStatus = lookupRegister(bank,REG_PIO_PDSR);
  return *(pinStatus) & pin ? 1 : 0;
}

inline void setPin(uint8_t bank, uint32_t pin, uint8_t llvl){
  if(llvl){
    uint32_t * stReg = lookupRegister(bank,REG_PIO_SODR);
    *stReg |= pin;
  }
  else{
    uint32_t * clrReg = lookupRegister(bank,REG_PIO_CODR);
    *clrReg |= pin;
  }
}

void setPullup(uint8_t bank, uint32_t pin, bool en){
  uint32_t * reg;
  if(en){
    reg = lookupRegister(bank,REG_PIO_PUER);
  }
  else {
    reg = lookupRegister(bank,REG_PIO_PUDR);
  }
  *reg |= pin;
}
