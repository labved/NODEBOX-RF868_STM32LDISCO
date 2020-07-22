/*
 * LIB_SDMMC.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: Engineeridea
 */
#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_SDMMC
  #define _USE_SDMMC
#endif

#if(_USE_SDMMC==1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "functions.h"

#include "stdlib.h"
#include "string.h"

#include "ff.h"
#include "LIB_SDMMC.h"
#include "HAL_SDCARD.h"


FATFS xfat;                                            	/* File system object */
FRESULT errCode;                                        /* Error code object  */
DIR directory;                                         /* Directory object   */
FIL fil_log;                                            /* Opened file object - LOG */
FIL fil_ex;                                             /* Opened file object - EX */

void SD_MMC::Init(void)
{
	// Setting GPIO for SCK
        RCC_AHBPeriphClockCmd(SDMMC_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(SDMMC_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals
        
	GPIO_InitTypeDef SPI_SDMMC_GPIO_CLK_InitStruct;

	SPI_SDMMC_GPIO_CLK_InitStruct.GPIO_Pin = SDMMC_PIN_CLK;
	SPI_SDMMC_GPIO_CLK_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SDMMC_GPIO_CLK_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SDMMC_GPIO_CLK_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	SPI_SDMMC_GPIO_CLK_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(SDMMC_GPIO_SPI, &SPI_SDMMC_GPIO_CLK_InitStruct);
       
	// Setting GPIO for MOSI
        RCC_AHBPeriphClockCmd(SDMMC_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(SDMMC_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SDMMC_GPIO_MOSI_InitStruct;

	SPI_SDMMC_GPIO_MOSI_InitStruct.GPIO_Pin = SDMMC_PIN_MOSI;
	SPI_SDMMC_GPIO_MOSI_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SDMMC_GPIO_MOSI_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SDMMC_GPIO_MOSI_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        SPI_SDMMC_GPIO_MOSI_InitStruct.GPIO_OType = GPIO_OType_PP;
        
	GPIO_Init(SDMMC_GPIO_SPI, &SPI_SDMMC_GPIO_MOSI_InitStruct);
        
        
	// Setting GPIO for MISO
        RCC_AHBPeriphClockCmd(SDMMC_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(SDMMC_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SDMMC_GPIO_MISO_InitStruct;

	SPI_SDMMC_GPIO_MISO_InitStruct.GPIO_Pin = SDMMC_PIN_MISO;
	SPI_SDMMC_GPIO_MISO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SDMMC_GPIO_MISO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SDMMC_GPIO_MISO_InitStruct.GPIO_OType = GPIO_OType_PP;
        SPI_SDMMC_GPIO_MISO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SDMMC_GPIO_SPI, &SPI_SDMMC_GPIO_MISO_InitStruct);

        // Setting AFF for CLK, MOSI, MISO
	GPIO_PinAFConfig(SDMMC_GPIO_SPI, SDMMC_AF_PIN_CLK, SDMMC_GPIO_AF);					//CLK
	GPIO_PinAFConfig(SDMMC_GPIO_SPI, SDMMC_AF_PIN_MISO, SDMMC_GPIO_AF);					//MISO
	GPIO_PinAFConfig(SDMMC_GPIO_SPI, SDMMC_AF_PIN_MOSI, SDMMC_GPIO_AF);                                     //MOSI
        
        // Setting GPIO for NSS
        RCC_AHBPeriphClockCmd(SDMMC_RCC_GPIO_NSS, ENABLE); // Setting RCC for BUSES
        RCC_APB2PeriphClockCmd(SDMMC_RCC_APB_NSS, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SDMMC_GPIO_NSS_InitStruct;

	SPI_SDMMC_GPIO_NSS_InitStruct.GPIO_Pin = SDMMC_PIN_NSS;
	SPI_SDMMC_GPIO_NSS_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_SDMMC_GPIO_NSS_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SDMMC_GPIO_NSS_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SDMMC_GPIO_NSS_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        
        GPIO_Init(SDMMC_GPIO_NSS, &SPI_SDMMC_GPIO_NSS_InitStruct);
       
        // Setting GPIO for CD
        RCC_AHBPeriphClockCmd(SDMMC_RCC_CD, ENABLE); // Enable GPIO clock 
	RCC_APB2PeriphClockCmd(SDMMC_CD_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
        
	GPIO_InitTypeDef SPI_SDMMC_GPIO_CD_InitStruct;

	SPI_SDMMC_GPIO_CD_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	SPI_SDMMC_GPIO_CD_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SDMMC_GPIO_CD_InitStruct.GPIO_Pin = SDMMC_PIN_CD;
	SPI_SDMMC_GPIO_CD_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_SDMMC_GPIO_CD_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(SDMMC_GPIO_CD, &SPI_SDMMC_GPIO_CD_InitStruct);
        
        GPIO_SetBits(SDMMC_GPIO_CD, SDMMC_PIN_CD);

	// Configuring SPI Settings
	//SPI_Cmd(SDMMC_SPI_CH, DISABLE);

	SPI_InitTypeDef  SPI_SDMMC_SPI_InitStruct;

	SPI_StructInit(&SPI_SDMMC_SPI_InitStruct);										// Setting SPI1 with default values

        SPI_SDMMC_SPI_InitStruct.SPI_Direction = SPI_SDMMC_DIRECTION;
        SPI_SDMMC_SPI_InitStruct.SPI_Mode = SPI_SDMMC_MODE;
        SPI_SDMMC_SPI_InitStruct.SPI_DataSize = SPI_SDMMC_DATA8_SIZE;
        SPI_SDMMC_SPI_InitStruct.SPI_CPOL = SPI_SDMMC_CPOL;
        SPI_SDMMC_SPI_InitStruct.SPI_CPHA = SPI_SDMMC_CPHA;
        SPI_SDMMC_SPI_InitStruct.SPI_NSS = SPI_SDMMC_NSS;
        SPI_SDMMC_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SDMMC_BAUD_SLOW_PRESCALER;    
        SPI_SDMMC_SPI_InitStruct.SPI_FirstBit = SPI_SDMMC_FIRSTBIT;

	SPI_Init(SDMMC_SPI_CH, &SPI_SDMMC_SPI_InitStruct);
        
	SPI_Cmd(SDMMC_SPI_CH, ENABLE);
        
        GPIO_SetBits(SDMMC_GPIO_NSS, SDMMC_PIN_NSS);
        delay_ms(10);
        GPIO_ResetBits(SDMMC_GPIO_NSS, SDMMC_PIN_NSS);
        delay_ms(10);
        GPIO_SetBits(SDMMC_GPIO_NSS, SDMMC_PIN_NSS);
        delay_ms(10);
}

void SD_MMC::clr_Buffer(char *buffer, unsigned int len)
{
	 unsigned int i=0;
	 for(i=0;i<=len;i++)
		 buffer[i]= 0x00;
}
FRESULT SD_MMC::format(void)
{
    errCode = FR_OK;
    return errCode;
}
FRESULT SD_MMC::makeVol(void)
{
  errCode = FR_OK;
  errCode = f_mount(&xfat, "", 1);							//mount drive number 0
  return errCode;
}
FRESULT SD_MMC::makeDir(char const *dir)
{
    errCode = FR_OK;
    errCode = f_mkdir(dir);
    return errCode;
}
FRESULT SD_MMC::dirOpen(char const *dir)
{
    errCode = FR_OK;
    errCode = f_opendir(&directory, dir);				        //root directory
    return errCode;
}
FRESULT SD_MMC::filClose(FIL fil_obj)
{
    errCode = FR_OK;
    errCode = f_close(&fil_obj);
    return errCode;
}
FRESULT SD_MMC::filWrite(FIL fil_obj, char *fil_path , char *buffer)
{
    errCode = FR_OK;
    UINT bytesWritten;

    errCode = f_open(&fil_obj, fil_path, FA_WRITE | FA_OPEN_ALWAYS);		// Open existing or create new file

    errCode = FR_OK;
    errCode = f_write(&fil_obj, buffer, strlen(buffer), &bytesWritten);

    errCode = FR_OK;
    errCode = f_close(&fil_obj);

    return errCode;
}
FRESULT SD_MMC::filRead(FIL fil_obj, char *fil_path , char *buffer)
{
    UINT bytesRead;
    //DWORD fsize;
    errCode = FR_OK;

    errCode = f_open(&fil_obj, fil_path, FA_READ);
    //fsize   = f_size(&fil_obj);

    errCode = FR_OK;
    errCode = f_read(&fil_obj, buffer, FATFS_BUFF, &bytesRead);

    errCode = FR_OK;
    errCode = f_close(&fil_obj);

    return errCode;
}
FRESULT SD_MMC::filburstWrite(FIL fil_obj, char const *fil_path, char *buffer)
{
    errCode = FR_OK;
    UINT bytesWritten;
    DWORD fsize;

    errCode = f_open(&fil_obj, fil_path, FA_WRITE | FA_OPEN_ALWAYS);		// Open existing or create new file

    if(errCode == FR_OK)
    	{
    		fsize = f_size(&fil_obj);
    		if(f_lseek(&fil_obj,fsize) == FR_OK)
                errCode = FR_OK;
    	}

    errCode = FR_OK;
    errCode = f_write(&fil_obj, buffer, strlen(buffer), &bytesWritten);
    
    errCode = FR_OK;
    errCode = f_close(&fil_obj);
    
    return errCode;
}
FRESULT SD_MMC::filburstRead(FIL fil_obj, char const *fil_path, char *buffer)
{
    errCode = FR_OK;
    UINT bytesRead;
    DWORD bytesptr = 0;
    DWORD bytestoRead = 0;
    DWORD fsize;
    signed int temp_flag = 0;

    errCode = f_open(&fil_obj, fil_path, FA_READ);				// Open existing or create new file
    fsize = f_size(&fil_obj);
    bytestoRead = fsize;

    if(errCode != FR_NO_FILE)
    {
        do
        {
          if(f_lseek(&fil_obj,bytesptr) == FR_OK)
            {
                    errCode = FR_OK;
                    clr_Buffer(buffer, FATFS_BUFF);
                    errCode = f_read(&fil_obj, buffer, FATFS_BUFF, &bytesRead);
                    bytesptr = bytesptr + bytesRead;
                    bytestoRead = bytestoRead - bytesRead;
                    if(bytestoRead < FATFS_BUFF)
                            temp_flag = 1;
                    else
                            bytesRead = 0;
            }
        } while(temp_flag != 1);


		clr_Buffer(buffer, FATFS_BUFF);
		errCode = FR_OK;
		errCode = f_read(&fil_obj, buffer, bytestoRead, &bytesRead);
  	}
  	errCode = FR_OK;
    errCode = f_close(&fil_obj);

    clr_Buffer(buffer, FATFS_BUFF);

    return errCode;
}

SD_MMC sdmmc;

#endif