/*******************************************************************************
 *
 *  HAL_SDCard.c - Driver for the SD Card slot
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_SDMMC
  #define _USE_SDMMC
#endif

#if(_USE_SDMMC==1)

#ifdef __cplusplus
extern "C" {
#endif



/***************************************************************************//**
 * @file       HAL_SDCard.c
 * @addtogroup HAL_SDCard
 * @{
 ******************************************************************************/

#include "HAL_SDCARD.h"
  
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Functions.h"

/***************************************************************************//**
 * @brief   Initialize SD Card
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_init(void)
{ 
 
        // Setting GPIO for SPI
        RCC_AHBPeriphClockCmd(SDMMC_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(SDMMC_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals
        
	// Setting GPIO for SCK
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
/***************************************************************************//**
 * @brief   Enable fast SD Card SPI transfers. This function is typically
 *          called after the initial SD Card setup is done to maximize
 *          transfer speed.
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_fastMode(void)
{
  // Configuring SPI Settings
  SPI_InitTypeDef  SPI_SDMMC_SPI_InitStruct;
  
  SPI_StructInit(&SPI_SDMMC_SPI_InitStruct);                                    // Setting SPI1 with default values
 
  SPI_SDMMC_SPI_InitStruct.SPI_Direction = SPI_SDMMC_DIRECTION;
  SPI_SDMMC_SPI_InitStruct.SPI_Mode = SPI_SDMMC_MODE;
  SPI_SDMMC_SPI_InitStruct.SPI_DataSize = SPI_SDMMC_DATA8_SIZE;
  SPI_SDMMC_SPI_InitStruct.SPI_CPOL = SPI_SDMMC_CPOL;
  SPI_SDMMC_SPI_InitStruct.SPI_CPHA = SPI_SDMMC_CPHA;
  SPI_SDMMC_SPI_InitStruct.SPI_NSS = SPI_SDMMC_NSS;
  SPI_SDMMC_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SDMMC_BAUD_FAST_PRESCALER;    
  SPI_SDMMC_SPI_InitStruct.SPI_FirstBit = SPI_SDMMC_FIRSTBIT;
    
  SPI_Init(SDMMC_SPI_CH, &SPI_SDMMC_SPI_InitStruct);
  
  SPI_Cmd(SDMMC_SPI_CH, ENABLE);
}

/***************************************************************************//**
 * @brief   Read a frame of bytes via SPI
 * @param   pBuffer Place to store the received bytes
 * @param   size Indicator of how many bytes to receive
 * @return  None
 ******************************************************************************/

void SDCard_readFrame(uint8_t *pBuffer, uint16_t size)
{
    SPI_I2S_ClearFlag(SDMMC_SPI_CH, SPI_I2S_FLAG_RXNE);
    //SPI_SDCARD_UCxxIFG &= ~SPI_SDCARD_UCRXIFG;                                   // Ensure RXIFG is clear

    // Clock the actual data transfer and receive the bytes
    while(size--)
    {
      while(!(SPI_I2S_GetFlagStatus(SDMMC_SPI_CH, SPI_I2S_FLAG_TXE)));                // Wait while not ready for TX
      SPI_I2S_SendData(SDMMC_SPI_CH, 0xff);                                          // Transmit Data
      while(SPI_I2S_GetFlagStatus(SDMMC_SPI_CH, SPI_I2S_FLAG_BSY));                   // Check if SPI Pherip is busy or Wait for TX to complete
      while(!(SPI_I2S_GetFlagStatus(SDMMC_SPI_CH, SPI_I2S_FLAG_RXNE)));               // Check if Rx Buffer is empty

      *pBuffer++ = SPI_I2S_ReceiveData(SDMMC_SPI_CH);
    }
}

/***************************************************************************//**
 * @brief   Send a frame of bytes via SPI
 * @param   pBuffer Place that holds the bytes to send
 * @param   size Indicator of how many bytes to send
 * @return  None
 ******************************************************************************/

void SDCard_sendFrame(uint8_t *pBuffer, uint16_t size)
{
    // Clock the actual data transfer and send the bytes. Note that we
    // intentionally not read out the receive buffer during frame transmission
    // in order to optimize transfer speed, however we need to take care of the
    // resulting overrun condition.
    while (size--){
        while(!(SPI_I2S_GetFlagStatus(SDMMC_SPI_CH, SPI_I2S_FLAG_TXE)));        // Wait while not ready for TX
        SPI_I2S_SendData(SDMMC_SPI_CH, *pBuffer++);                             // Transmit Data
    }
    while(SPI_I2S_GetFlagStatus(SDMMC_SPI_CH, SPI_I2S_FLAG_BSY));               // Check if SPI Pherip is busy or Wait for TX to complete

    SPI_I2S_ReceiveData(SDMMC_SPI_CH);                                          // Dummy read to empty RX buffer
                                                                                // and clear any overrun conditions
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to high
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSHigh(void)
{
    GPIO_SetBits(SDMMC_GPIO_NSS, SDMMC_PIN_NSS); 
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to low
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSLow(void)
{
    GPIO_ResetBits(SDMMC_GPIO_NSS, SDMMC_PIN_NSS); 
}

/***************************************************************************//**
 * @}
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
