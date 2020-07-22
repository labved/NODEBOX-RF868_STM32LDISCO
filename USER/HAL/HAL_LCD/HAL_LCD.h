#include "stm32l1xx.h"
#include "Settings.h"

#ifndef HAL_LCD_H
  #define HAL_LCD_H

#if (_USE_LCD == 1)

//***************************************LCD define**************************************************//
#define byte char
#define uint unsigned int
#define uchar unsigned char

class HAL_ERC2401602
{
public:
	uchar SPITransfer(uchar value);
	void SPIStrobe(uchar strobe);
	void Periph_Init(void);
	void SPIInit(void);

	void write_com(uchar idata_cmd);
	void write_data(uchar dat);
};

extern HAL_ERC2401602 lcd240160;

#endif

#endif
