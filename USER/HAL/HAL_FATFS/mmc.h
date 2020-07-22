#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_SDMMC
  #define _USE_SDMMC
#endif

#if(_USE_SDMMC==1)
uint8_t detectCard(void);

#endif