#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_FLASH
  #define _USE_FLASH
#endif

#if(_USE_FLASH==1)

#include "Settings.h"
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Functions.h"
#include "Conversions.h"

#include "stdint.h"

#include "HAL_W25Q.h"

/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void HAL_W25Q::SPIStrobe(uchar strobe)
{
	SPIModeWrite();
	delay_ms(1);

	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
	delay_ms(1);

	while (GPIO_ReadInputDataBit(SFLASH_GPIO_SPI, SFLASH_PIN_MOSI))
		;
	SPITransfer(strobe);

	delay_ms(1);
	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
uchar HAL_W25Q::SPITransfer(uchar value)
{
	uchar buffer;

	while (!(SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_TXE)))
		; // Check if Tx Buffer is empty

	SPI_I2S_SendData(SFLASH_SPI_CH, value); // Transmit Data
	while (SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_BSY))
		; // Check if SPI Pherip is busy or Wait for TX to complete
	while (!(SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_RXNE)))
		; // Check if Rx Buffer is empty

	buffer = SPI_I2S_ReceiveData(SFLASH_SPI_CH);

	return buffer;
}

/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
uchar HAL_W25Q::SPIReadTransfer(uchar value)
{
	uchar buffer;

	SPIModeRead();

	while (!(SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_TXE)))
		;									// Check if Tx Buffer is empty
	SPI_I2S_SendData(SFLASH_SPI_CH, value); // Transmit Data
	while (SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_BSY))
		; // Check if SPI Pherip is busy or Wait for TX to complete
	while (!(SPI_I2S_GetFlagStatus(SFLASH_SPI_CH, SPI_I2S_FLAG_RXNE)))
		; // Check if Rx Buffer is empty

	buffer = SPI_I2S_ReceiveData(SFLASH_SPI_CH);

	return buffer;
}

/****************************************************************
*FUNCTION NAME:SPIModeWrite
*FUNCTION     :SPIModeWrite
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_W25Q::SPIModeWrite(void)
{
	SPI_Cmd(SFLASH_SPI_CH, DISABLE);

	// Configuring SPI Settings
	SPI_InitTypeDef SPI_SFLASH_SPI_InitStruct;

	SPI_StructInit(&SPI_SFLASH_SPI_InitStruct); // Setting SPI1 with default values

	SPI_SFLASH_SPI_InitStruct.SPI_Direction = SPI_SFLASH_DIRECTION;
	SPI_SFLASH_SPI_InitStruct.SPI_Mode = SPI_SFLASH_MODE;
	SPI_SFLASH_SPI_InitStruct.SPI_DataSize = SPI_SFLASH_DATA_SIZE;
	SPI_SFLASH_SPI_InitStruct.SPI_CPOL = SPI_SFLASH_CPOL;
	SPI_SFLASH_SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_SFLASH_SPI_InitStruct.SPI_NSS = SPI_SFLASH_NSS;
	SPI_SFLASH_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SFLASH_BAUD_PRESCALER;
	SPI_SFLASH_SPI_InitStruct.SPI_FirstBit = SPI_SFLASH_FIRSTBIT;

	SPI_Init(SFLASH_SPI_CH, &SPI_SFLASH_SPI_InitStruct);
	//delay_ms(10);

	SPI_Cmd(SFLASH_SPI_CH, ENABLE);
}

/****************************************************************
*FUNCTION NAME:SPIModeRead
*FUNCTION     :SPIModeRead
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_W25Q::SPIModeRead(void)
{
	// Configuring SPI Settings
	SPI_InitTypeDef SPI_SFLASH_SPI_InitStruct;

	SPI_StructInit(&SPI_SFLASH_SPI_InitStruct); // Setting SPI1 with default values

	SPI_SFLASH_SPI_InitStruct.SPI_Direction = SPI_SFLASH_DIRECTION;
	SPI_SFLASH_SPI_InitStruct.SPI_Mode = SPI_SFLASH_MODE;
	SPI_SFLASH_SPI_InitStruct.SPI_DataSize = SPI_SFLASH_DATA_SIZE;
	SPI_SFLASH_SPI_InitStruct.SPI_CPOL = SPI_SFLASH_CPOL;
	SPI_SFLASH_SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_SFLASH_SPI_InitStruct.SPI_NSS = SPI_SFLASH_NSS;
	SPI_SFLASH_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SFLASH_BAUD_PRESCALER;
	SPI_SFLASH_SPI_InitStruct.SPI_FirstBit = SPI_SFLASH_FIRSTBIT;

	SPI_Init(SFLASH_SPI_CH, &SPI_SFLASH_SPI_InitStruct);

	SPI_Cmd(SFLASH_SPI_CH, ENABLE);
}

/****************************************************************
*FUNCTION NAME:SPIInit
*FUNCTION     :spi communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_W25Q::SPIInit(void)
{

	// Setting GPIO for SPI
	RCC_AHBPeriphClockCmd(SFLASH_RCC_GPIO, ENABLE); // Setting RCC for BUSES
	RCC_APB2PeriphClockCmd(SFLASH_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

        // Setting GPIO for MOSI
	GPIO_InitTypeDef SPI_SFLASH_GPIO_CLK_InitStruct;

	SPI_SFLASH_GPIO_CLK_InitStruct.GPIO_Pin = SFLASH_PIN_CLK;
	SPI_SFLASH_GPIO_CLK_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SFLASH_GPIO_CLK_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SFLASH_GPIO_CLK_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	SPI_SFLASH_GPIO_CLK_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(SFLASH_GPIO_SPI, &SPI_SFLASH_GPIO_CLK_InitStruct);

	// Setting GPIO for MOSI
	RCC_AHBPeriphClockCmd(SFLASH_RCC_GPIO, ENABLE); // Setting RCC for BUSES
	RCC_APB2PeriphClockCmd(SFLASH_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SFLASH_GPIO_MOSI_InitStruct;

	SPI_SFLASH_GPIO_MOSI_InitStruct.GPIO_Pin = SFLASH_PIN_MOSI;
	SPI_SFLASH_GPIO_MOSI_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SFLASH_GPIO_MOSI_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SFLASH_GPIO_MOSI_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SFLASH_GPIO_MOSI_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SFLASH_GPIO_SPI, &SPI_SFLASH_GPIO_MOSI_InitStruct);

	// Setting GPIO for MISO
	RCC_AHBPeriphClockCmd(SFLASH_RCC_GPIO, ENABLE); // Setting RCC for BUSES
	RCC_APB2PeriphClockCmd(SFLASH_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SFLASH_GPIO_MISO_InitStruct;

	SPI_SFLASH_GPIO_MISO_InitStruct.GPIO_Pin = SFLASH_PIN_MISO;
	SPI_SFLASH_GPIO_MISO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_SFLASH_GPIO_MISO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SFLASH_GPIO_MISO_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SFLASH_GPIO_MISO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SFLASH_GPIO_SPI, &SPI_SFLASH_GPIO_MISO_InitStruct);
        
        // Setting AF for CLK, MOSI, MISO
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_CLK, SPI_SFLASH_GPIO_AF);					//CLK
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_MISO, SPI_SFLASH_GPIO_AF);					//MISO
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_MOSI, SPI_SFLASH_GPIO_AF);					//MOSI

	// Setting GPIO for NSS
	RCC_AHBPeriphClockCmd(SFLASH_RCC_NSS, ENABLE);			// Setting RCC for BUSES
	RCC_APB2PeriphClockCmd(SFLASH_NSS_SYSCONFIG_APB, ENABLE);       // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_SFLASH_GPIO_NSS_InitStruct;

	SPI_SFLASH_GPIO_NSS_InitStruct.GPIO_Pin = SFLASH_PIN_NSS;
	SPI_SFLASH_GPIO_NSS_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_SFLASH_GPIO_NSS_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_SFLASH_GPIO_NSS_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SFLASH_GPIO_NSS_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SFLASH_GPIO_NSS, &SPI_SFLASH_GPIO_NSS_InitStruct);

	// Setting GPIO for WP
	RCC_AHBPeriphClockCmd(SFLASH_RCC_WP, ENABLE);			   // Enable GPIO clock
	RCC_APB2PeriphClockCmd(SFLASH_HOLD_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock

	GPIO_InitTypeDef SPI_SFLASH_GPIO_WP_InitStruct;

	SPI_SFLASH_GPIO_WP_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_SFLASH_GPIO_WP_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SFLASH_GPIO_WP_InitStruct.GPIO_Pin = SFLASH_PIN_WP;
	SPI_SFLASH_GPIO_WP_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_SFLASH_GPIO_WP_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(SFLASH_GPIO_WP, &SPI_SFLASH_GPIO_WP_InitStruct);

	GPIO_SetBits(SFLASH_GPIO_WP, SFLASH_PIN_WP);

	// Setting GPIO HOLD
	RCC_AHBPeriphClockCmd(SFLASH_RCC_HOLD, ENABLE);			   // Enable GPIO clock
	RCC_APB2PeriphClockCmd(SFLASH_HOLD_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock

	GPIO_InitTypeDef SPI_SFLASH_GPIO_HOLD_InitStruct;

	SPI_SFLASH_GPIO_HOLD_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_SFLASH_GPIO_HOLD_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_SFLASH_GPIO_HOLD_InitStruct.GPIO_Pin = SFLASH_PIN_HOLD;
	SPI_SFLASH_GPIO_HOLD_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_SFLASH_GPIO_HOLD_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(SFLASH_GPIO_HOLD, &SPI_SFLASH_GPIO_HOLD_InitStruct);

	// Setting GPIO for CLK, MOSI, MISO
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_CLK, SPI_SFLASH_GPIO_AF);  //CLK
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_MISO, SPI_SFLASH_GPIO_AF); //MISO
	GPIO_PinAFConfig(SFLASH_GPIO_SPI, SFLASH_AF_PIN_MOSI, SPI_SFLASH_GPIO_AF); //MOSI

	// Configuring SPI Settings
	SPI_Cmd(SFLASH_SPI_CH, DISABLE);

	SPI_InitTypeDef SPI_SFLASH_SPI_InitStruct;

	SPI_StructInit(&SPI_SFLASH_SPI_InitStruct); // Setting SPI1 with default values

	SPI_SFLASH_SPI_InitStruct.SPI_Direction = SPI_SFLASH_DIRECTION;
	SPI_SFLASH_SPI_InitStruct.SPI_Mode = SPI_SFLASH_MODE;
	SPI_SFLASH_SPI_InitStruct.SPI_DataSize = SPI_SFLASH_DATA_SIZE;
	SPI_SFLASH_SPI_InitStruct.SPI_CPOL = SPI_SFLASH_CPOL;
	SPI_SFLASH_SPI_InitStruct.SPI_CPHA = SPI_SFLASH_CPHA;
	SPI_SFLASH_SPI_InitStruct.SPI_NSS = SPI_SFLASH_NSS;
	SPI_SFLASH_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SFLASH_BAUD_PRESCALER;
	SPI_SFLASH_SPI_InitStruct.SPI_FirstBit = SPI_SFLASH_FIRSTBIT;

	SPI_Init(SFLASH_SPI_CH, &SPI_SFLASH_SPI_InitStruct);

	SPI_Cmd(SFLASH_SPI_CH, ENABLE);

	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
       	GPIO_ResetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);
       	GPIO_SetBits(SFLASH_GPIO_NSS, SFLASH_PIN_NSS);


}
#endif