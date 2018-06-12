#include "general.h"
#include <sam3xa/include/sam3x8e.h>

void setupMainClock(void){
  // ???? Mhz
  EFC0->EEFC_FMR = EEFC_FMR_FWS(4);
  EFC1->EEFC_FMR = EEFC_FMR_FWS(4);

  PMC->CKGR_MOR = CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY(0x37) | CKGR_MOR_MOSCXTST(0x8u);

  //wait for clock to start
  while(!(PMC->PMC_SR & PMC_SR_MOSCXTS));

  //select clock
  PMC->CKGR_MOR = CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY(0x37) | CKGR_MOR_MOSCXTST(0x8u);

  //wait for select
  while(!(PMC->PMC_SR & PMC_SR_MOSCSELS));

  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;

  while(!(PMC->PMC_SR & PMC_SR_MCKRDY));


  PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0xdUL) | CKGR_PLLAR_DIVA(0x1UL) | CKGR_PLLAR_PLLACOUNT(0x3fU);

  //wait for lock
  while(!(PMC->PMC_SR & PMC_SR_LOCKA));


  PMC->PMC_MCKR = PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_MAIN_CLK;

  while(!(PMC->PMC_SR & PMC_SR_MCKRDY));

  PMC->PMC_MCKR = PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK;

  while(!(PMC->PMC_SR & PMC_SR_MCKRDY));

  //we now should now be at 84 Mhz
}

// start random number generator
void startTRNG(void){
  REG_PMC_PCER1 |= (1 << 9); // 41 is TRANG peripheral id
  TRNG->TRNG_CR = TRNG_CR_KEY(0x524e47) | TRNG_CR_ENABLE;
}

//get random 32 bit number from TRNG
uint32_t random(void){
  return TRNG->TRNG_ODATA;
}
