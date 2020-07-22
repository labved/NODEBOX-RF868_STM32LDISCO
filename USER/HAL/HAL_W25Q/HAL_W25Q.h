#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_FLASH
  #define _USE_FLASH
#endif

#if(_USE_FLASH==1)

//*************************************** DEFINE **************************************************//
#define uchar unsigned char

class HAL_W25Q
{
public:
        void SPIStrobe(uchar strobe);
        uchar SPITransfer(uchar value);
        uchar SPIReadTransfer(uchar val);
        uchar SPIWriteTransfer(uchar val);
        void SPIInit(void);
        void SPIModeWrite(void);
        void SPIModeRead(void);
};
extern HAL_W25Q w25q;


#endif