/*
ELECHOUSE_CC1101.cpp - CC1101 module library
Copyright (c) 2010 Michael.
Author: Michael, <www.elechouse.com>
Version: November 12, 2010

This library is designed to use CC1101/CC1100 module on Arduino platform.
CC1101/CC1100 module is an useful wireless module.Using the functions of the
library, you can easily send and receive data by the CC1101/CC1100 module.
Just have fun!
For the details, please refer to the datasheet of CC1100/CC1101.
*/

#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_FLASH
  #define _USE_FLASH
#endif

#if(_USE_FLASH==1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"
#include "Functions.h"
#include "Conversions.h"

#include "HAL_W25Q.h"
#include "LIB_FLASH.h"

#include "stdint.h"

HAL_W25Q w25q;


/****************************************************************
*FUNCTION NAME:PoweronReset
*FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::PoweronReset(void)
{
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(10);
	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(10);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(10);
        
        w25q.SPIModeWrite();
	delay_ms(10);

	wakeUp();
	delay_ms(10);

}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :LCD register config //details refer datasheet of ST7586S//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::RegConfigSettings(void)
{


}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :LCD initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::Init()
{
        w25q.SPIInit();
	PoweronReset();
	RegConfigSettings();
}

/****************************************************************
*FUNCTION NAME:readStatusRegister
*FUNCTION     :readStatusRegister
*INPUT        :none
*OUTPUT       :none
****************************************************************/
uchar LIB_QFLASH::readStatusRegister(uchar reg) 
{
	uchar temp;

	w25q.SPIModeWrite();
	delay_ms(1);
        
	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);
        
	w25q.SPITransfer(reg);
        
	w25q.SPIModeRead();
	temp = w25q.SPITransfer(0x00);               // Dummy write

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

	return temp;
}
/****************************************************************
*FUNCTION NAME:writeStatusRegister
*FUNCTION     :writeStatusRegister
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::writeStatusRegister(uchar reg, uchar val)
{
	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(reg);

	w25q.SPITransfer(val);

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:readData
*FUNCTION     :readData
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::readData(uchar *val, uchar *addrs, uint32_t bytes)
{
	uint32_t i;

	w25q.SPIModeWrite();
	delay_ms(1);
        
	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_READ_DATA);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	w25q.SPIModeRead();

	for (i = 0; i < bytes; i++)
	{
		val[i] = w25q.SPITransfer(0x00);				// Dummy write
	}

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

}

/****************************************************************
*FUNCTION NAME:fastReadData
*FUNCTION     :fastReadData
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::fastReadData(uchar *val, uchar *addrs, uint32_t bytes)
{
	uint32_t i;

	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_FAST_READ_DATA);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	w25q.SPITransfer(0x00);							// Dummy write

	w25q.SPIModeRead();

	for (i = 0; i < bytes; i++)
	{
		val[i] = w25q.SPITransfer(0x00);			// Dummy write
	}

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}
	
/****************************************************************
*FUNCTION NAME:pageProgram
*FUNCTION     :pageProgram
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::pageProgram(uchar *val, uchar *addrs, uint32_t bytes)
{
	uint32_t i;

	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_PROG_PAGE);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	for (i = 0; i < bytes; i++)
	{
		w25q.SPITransfer(val[i]);					// Writing data byte
	}

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:eraseSector
*FUNCTION     :eraseSector
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::eraseSector(uchar *addrs)
{
  	uint8_t i;
	uchar reg;

	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_ERASE_SECTOR);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(400);								// Wait until Tse is completed


	do
	{
		reg = readStatusRegister(SFLASH_READ_SR1);
		reg = reg & SFLASH_SR1_MASK_BUSY;

	} while (reg);


}

/****************************************************************
*FUNCTION NAME:eraseBlock32
*FUNCTION     :eraseBlock32
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::eraseBlock32K(uchar *addrs)
{
    uint8_t i;
	uchar reg;

	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_ERASE_BLOCK32);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(2);								// Wait until Tbe1 is completed

	do
	{
		reg = readStatusRegister(SFLASH_READ_SR1);
		reg = reg & SFLASH_SR1_MASK_BUSY;

	} while (reg);

}

/****************************************************************
*FUNCTION NAME:eraseBlock64
*FUNCTION     :eraseBlock64
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::eraseBlock64K(uchar *addrs)
{
    uint8_t i;
	uchar reg;

	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_ERASE_BLOCK64);

	for (i = 0; i < 3; i++)						// Writing 24 bit address MSB first
	{
		w25q.SPITransfer(addrs[i]);
	}

	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(2);								// Wait until Tbe2 is completed

	do
	{
		reg = readStatusRegister(SFLASH_READ_SR1);
		reg = reg & SFLASH_SR1_MASK_BUSY;

	} while (reg);
}

/****************************************************************
*FUNCTION NAME:eraseChip
*FUNCTION     :eraseChip
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::eraseChip(void)
{
	uchar reg;
       
	w25q.SPIStrobe(SFLASH_WRITE_EN);
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_ERASE_CHIP);

	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(100);									// Wait until TCe is complete

	do
	{
		reg = readStatusRegister(SFLASH_READ_SR1);
		reg = reg & SFLASH_SR1_MASK_BUSY;

	} while (reg);


}

/****************************************************************
*FUNCTION NAME:powerDown
*FUNCTION     :powerDown
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::powerDown(void)
{
	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(10);

	w25q.SPITransfer(SFLASH_PWR_DOWN);

	delay_ms(10);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

	delay_us(200);												// Keep CSn high tDP
}

/****************************************************************
*FUNCTION NAME:wakeUp
*FUNCTION     :wakeUp
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::wakeUp(void)
{
	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(10);

	w25q.SPITransfer(SFLASH_DEV_ID);

	delay_ms(10);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

	delay_us(10);												// Keep CSn high tRES1
}

/****************************************************************
*FUNCTION NAME:getDeviceID
*FUNCTION     :getDeviceID
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::getDeviceID(uchar &val)
{
	uint8_t i = 0;

	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_DEV_ID);

	for (i = 0; i < 3; i++)						// Writing 24 bit dummy
	{
		w25q.SPITransfer(0x00);						// Write dummy
	}
        
    
	w25q.SPIModeRead();

	val = w25q.SPITransfer(0x00);					// Write dummy - return Device ID

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

	delay_us(200);								// Keep CSn high tRES2
        
}

/****************************************************************
*FUNCTION NAME:getManufactureID
*FUNCTION     :getManufactureID
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::getManufacturerID(uchar *val)
{
    uint8_t i = 0;
	
	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_MAN_ID);

	for (i = 0; i < 3; i++)						// Writing 24 bit dummy
	{
		w25q.SPITransfer(0x00);						// Write dummy 
	}
        
        w25q.SPIModeRead();

	val[0] = w25q.SPITransfer(0x00);				// Write dummy - return Manufacturer ID
	val[1] = w25q.SPITransfer(0x00);				// Write dummy - return Device ID

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);

}

/****************************************************************
*FUNCTION NAME:getUniqueID
*FUNCTION     :getUniqueID
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::getUniqueID(uchar *val)
{
    uint8_t i;

        w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_UNIQUE_ID);

	for (i = 0; i < 4; i++)						// Writing 32 bit / 4 Bytes dummy
	{
		w25q.SPITransfer(0x00);						// Write dummy 
	}
        
	w25q.SPIModeRead();

	for (i = 0; i < 8; i++)						// Writing dummy 8 bytes
	{
		val[i] = w25q.SPITransfer(0x00);				// Write dummy 
	}

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:getJedecID
*FUNCTION     :getJedecID
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::getJedecID(uchar *val)
{
	w25q.SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	w25q.SPITransfer(SFLASH_JEDEC_ID);

	w25q.SPIModeRead();

	val[0] = w25q.SPITransfer(0x00);				// Write dummy - return Manufacturer ID
	val[1] = w25q.SPITransfer(0x00);				// Write dummy - return Memory Type
	val[2] = w25q.SPITransfer(0x00);				// Write dummy - return Memory Capacity

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:addressto3Byte
*FUNCTION     :addressto3Byte
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_QFLASH::addressto3BYTES(uint32_t addrs, uchar *val)
{
	val[0] = (SFLASH_ADDRS_MASK_A0 & addrs) >> 16;
	val[1] = (SFLASH_ADDRS_MASK_A1 & addrs) >> 8;
	val[2] = SFLASH_ADDRS_MASK_A2 & addrs;
}

LIB_QFLASH qFlash;

#endif