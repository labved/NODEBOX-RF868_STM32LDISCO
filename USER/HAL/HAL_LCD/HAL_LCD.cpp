#include "stm32l1xx.h"  
#include "Settings.h"


#ifndef _USE_LCD
  #define _USE_LCD
#endif

#if(_USE_LCD==1) 

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_LCD.h"




/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void HAL_ERC2401602::SPIStrobe(uchar strobe)
{
	GPIO_ResetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
	delay_us(50);

	while (GPIO_ReadInputDataBit(LCD_GPIO_SPI, LCD_PIN_MISO));
	SPITransfer(strobe);

	delay_us(50);
	GPIO_SetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
uchar HAL_ERC2401602::SPITransfer(uchar value)
{
	uchar buffer;
	while (!(SPI_I2S_GetFlagStatus(LCD_SPI_CH, SPI_I2S_FLAG_TXE)));					// Check if Tx Buffer is empty
	SPI_I2S_SendData(LCD_SPI_CH, value);								// Transmit Data
	while (SPI_I2S_GetFlagStatus(LCD_SPI_CH, SPI_I2S_FLAG_BSY));					// Check if SPI Pherip is busy or Wait for TX to complete
	while (!(SPI_I2S_GetFlagStatus(LCD_SPI_CH, SPI_I2S_FLAG_RXNE)));				// Check if Rx Buffer is empty

	buffer = SPI_I2S_ReceiveData(LCD_SPI_CH);

	return buffer;
}

/****************************************************************
*FUNCTION NAME: Periph_Init()
*FUNCTION     : set RS, DI pin
*INPUT        : none
*OUTPUT       : none
****************************************************************/
void HAL_ERC2401602::Periph_Init(void)
{
	// RST CONFIGURATION


	

}

/****************************************************************
*FUNCTION NAME:SPIInit
*FUNCTION     :spi communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_ERC2401602::SPIInit(void)
{
        // Setting GPIO for SCK
        RCC_AHBPeriphClockCmd(LCD_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(LCD_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals
        
	GPIO_InitTypeDef SPI_LCD_GPIO_CLK_InitStruct;

	SPI_LCD_GPIO_CLK_InitStruct.GPIO_Pin = LCD_PIN_CLK;
	SPI_LCD_GPIO_CLK_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_LCD_GPIO_CLK_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_LCD_GPIO_CLK_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	SPI_LCD_GPIO_CLK_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(LCD_GPIO_SPI, &SPI_LCD_GPIO_CLK_InitStruct);
       
	// Setting GPIO for MOSI
        RCC_AHBPeriphClockCmd(LCD_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(LCD_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_LCD_GPIO_MOSI_InitStruct;

	SPI_LCD_GPIO_MOSI_InitStruct.GPIO_Pin = LCD_PIN_MOSI;
	SPI_LCD_GPIO_MOSI_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_LCD_GPIO_MOSI_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_LCD_GPIO_MOSI_InitStruct.GPIO_OType = GPIO_OType_PP;
        SPI_LCD_GPIO_MOSI_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(LCD_GPIO_SPI, &SPI_LCD_GPIO_MOSI_InitStruct);
        
        
	// Setting GPIO for MISO
        RCC_AHBPeriphClockCmd(LCD_RCC_GPIO, ENABLE); // Setting RCC for BUSES
        RCC_APB1PeriphClockCmd(LCD_RCC_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_LCD_GPIO_MISO_InitStruct;

	SPI_LCD_GPIO_MISO_InitStruct.GPIO_Pin = LCD_PIN_MISO;
	SPI_LCD_GPIO_MISO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	SPI_LCD_GPIO_MISO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_LCD_GPIO_MISO_InitStruct.GPIO_OType = GPIO_OType_PP;
        SPI_LCD_GPIO_MISO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(LCD_GPIO_SPI, &SPI_LCD_GPIO_MISO_InitStruct);
        
        // Setting GPIO for CSn
        RCC_AHBPeriphClockCmd(LCD_RCC_GPIO_NSS, ENABLE); // Setting RCC for BUSES
        RCC_APB2PeriphClockCmd(LCD_NSS_SYSCONFIG_APB, ENABLE); // Setting RCC for BUSES - Peripherals

	GPIO_InitTypeDef SPI_LCD_GPIO_NSS_InitStruct;

	SPI_LCD_GPIO_NSS_InitStruct.GPIO_Pin = LCD_PIN_NSS;
	SPI_LCD_GPIO_NSS_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_LCD_GPIO_NSS_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	SPI_LCD_GPIO_NSS_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_LCD_GPIO_NSS_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        
        GPIO_Init(LCD_GPIO_NSS, &SPI_LCD_GPIO_NSS_InitStruct);
        
        // Setting GPIO for RES
        RCC_AHBPeriphClockCmd(LCD_RCC_RST, ENABLE); // Enable GPIO clock 
	RCC_APB2PeriphClockCmd(LCD_RST_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
        
	GPIO_InitTypeDef SPI_LCD_GPIO_RST_InitStruct;

	SPI_LCD_GPIO_RST_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_LCD_GPIO_RST_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_LCD_GPIO_RST_InitStruct.GPIO_Pin = LCD_PIN_RST;
	SPI_LCD_GPIO_RST_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_LCD_GPIO_RST_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(LCD_GPIO_RST, &SPI_LCD_GPIO_RST_InitStruct);
        
        GPIO_SetBits(LCD_GPIO_RST, LCD_PIN_RST);


	// A0 or DI CONFIGURATION
	RCC_AHBPeriphClockCmd(LCD_RCC_A0, ENABLE); 	// Enable GPIO clock 
	RCC_APB2PeriphClockCmd(LCD_A0_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 

	// Setting GPIO for DI
	GPIO_InitTypeDef SPI_LCD_GPIO_A0_InitStruct;

	SPI_LCD_GPIO_A0_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	SPI_LCD_GPIO_A0_InitStruct.GPIO_OType = GPIO_OType_PP;
	SPI_LCD_GPIO_A0_InitStruct.GPIO_Pin = LCD_PIN_A0;
	SPI_LCD_GPIO_A0_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_LCD_GPIO_A0_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(LCD_GPIO_A0, &SPI_LCD_GPIO_A0_InitStruct);

	// Setting GPIO for CLK, MOSI, MISO
	GPIO_PinAFConfig(LCD_GPIO_SPI, LCD_AF_PIN_CLK, LCD_GPIO_AF);					//CLK
	GPIO_PinAFConfig(LCD_GPIO_SPI, LCD_AF_PIN_MISO, LCD_GPIO_AF);					//MISO
	GPIO_PinAFConfig(LCD_GPIO_SPI, LCD_AF_PIN_MOSI, LCD_GPIO_AF);					//MOSI

	// Configuring SPI Settings
	SPI_Cmd(LCD_SPI_CH, DISABLE);

	SPI_InitTypeDef  SPI_LCD_SPI_InitStruct;

	SPI_StructInit(&SPI_LCD_SPI_InitStruct);										// Setting SPI1 with default values

	SPI_LCD_SPI_InitStruct.SPI_Direction = SPI_LCD_DIRECTION;
	SPI_LCD_SPI_InitStruct.SPI_Mode = SPI_LCD_MODE;
	SPI_LCD_SPI_InitStruct.SPI_DataSize = SPI_LCD_DATA_SIZE;
	SPI_LCD_SPI_InitStruct.SPI_CPOL = SPI_LCD_CPOL;
	SPI_LCD_SPI_InitStruct.SPI_CPHA = SPI_LCD_CPHA;
	SPI_LCD_SPI_InitStruct.SPI_NSS = SPI_LCD_NSS;
	SPI_LCD_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_LCD_BAUD_PRESCALER;
	SPI_LCD_SPI_InitStruct.SPI_FirstBit = SPI_LCD_FIRSTBIT;

	SPI_Init(LCD_SPI_CH, &SPI_LCD_SPI_InitStruct);
        
	SPI_Cmd(LCD_SPI_CH, ENABLE);
        
        GPIO_SetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
}

/****************************************************************
*FUNCTION NAME:write_com
*FUNCTION     : Writes Command
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_ERC2401602::write_com(uchar cmd)
{

	GPIO_ResetBits(LCD_GPIO_A0, LCD_PIN_A0);
	GPIO_ResetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
    delay_us(50);

	lcd240160.SPITransfer(cmd);

	GPIO_SetBits(LCD_GPIO_NSS, LCD_PIN_NSS);

}

/****************************************************************
*FUNCTION NAME:write_data
*FUNCTION     :	Write Data
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_ERC2401602::write_data(uchar dat)
{

	GPIO_SetBits(LCD_GPIO_A0, LCD_PIN_A0);
	GPIO_ResetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
	delay_us(50);

	lcd240160.SPITransfer(dat);

	GPIO_SetBits(LCD_GPIO_NSS, LCD_PIN_NSS);

}
#endif


