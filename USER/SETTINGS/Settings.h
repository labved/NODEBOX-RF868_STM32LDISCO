#include "stm32l1xx.h"

#ifndef SETTINGS_H
  #define SETTINGS_H

/* COMPILER INFORMATION           */
#define _USE_LORA 1
#define _USE_GSM 1
#define _USE_FLASH 1
#define _USE_SDMMC 1
#define _USE_LCD 1
#define _USE_BT 1
#define _USE_IOEXP 1
#define _USE_MODEL 1

/* VOLUME INFORMATION           */
#define SENSOR_PACK 10 // 10 No's Pack
#define ZONE_PACK 10
#define RELAY_PACK 2
#define ONOFF_PACK 5
#define CMP_PACK 5

/* FRAME DEFINITION        */
#define MAX_BUFFLEN 64
#define NAME_BYTE 16
#define ADRS_BYTE 1

// SYSTEM CLOCK
#define CLK_SRC RCC_SYSCLKSource_HSI
#define CLK_SYSCLK_PRESCALER RCC_SYSCLK_Div1 // 16MHZ
#define CLK_PCLK1_PRESCALER RCC_HCLK_Div2    // 16/2 = 8 MHZ
#define CLK_PCLK2_PRESCALER RCC_HCLK_Div2    // 16/2 = 8 MHZ

#define CLK_MCO_SRC RCC_MCOSource_HSI
#define CLK_MCO_PRESCALER RCC_MCODiv_1

// SPI SETTINGS
#define SPI_CC1101_DIRECTION SPI_Direction_2Lines_FullDuplex
#define SPI_CC1101_MODE SPI_Mode_Master
#define SPI_CC1101_DATA_SIZE SPI_DataSize_8b
#define SPI_CC1101_CPOL SPI_CPOL_Low
#define SPI_CC1101_CPHA SPI_CPHA_1Edge
#define SPI_CC1101_NSS SPI_NSS_Soft | SPI_NSSInternalSoft_Set
#define SPI_CC1101_BAUD_PRESCALER SPI_BaudRatePrescaler_2 // APB = 8MHZ, Hence 8/2 = 4MBPS
#define SPI_CC1101_FIRSTBIT SPI_FirstBit_MSB

#define CC1101_NVIC_CHAN_GDO0 EXTI9_5_IRQn
#define CC1101_NVIC_PREPRIOR_GDO0 0x00
#define CC1101_NVIC_SUBPRIOR_GDO0 0x00

// SPI SETTINGS
#define SPI_SDMMC_DIRECTION SPI_Direction_2Lines_FullDuplex
#define SPI_SDMMC_MODE SPI_Mode_Master
#define SPI_SDMMC_DATA8_SIZE SPI_DataSize_8b
#define SPI_SDMMC_DATA16_SIZE SPI_DataSize_16b
#define SPI_SDMMC_CPOL SPI_CPOL_Low
#define SPI_SDMMC_CPHA SPI_CPHA_1Edge
#define SPI_SDMMC_NSS SPI_NSS_Soft | SPI_NSSInternalSoft_Set
#define SPI_SDMMC_BAUD_SLOW_PRESCALER SPI_BaudRatePrescaler_4 // APB = 8MHZ, Hence 8/4 = 2MBPS
#define SPI_SDMMC_BAUD_FAST_PRESCALER SPI_BaudRatePrescaler_2 // APB = 8MHZ, Hence 8/2 = 4MBPS
#define SPI_SDMMC_FIRSTBIT SPI_FirstBit_MSB

#define DIR_LOG "/log"
#define DIR_SETTINGS "/settings"
#define DIR_RELAY "/relay"
#define DIR_SENSOR "/sensor"

#define FILE_PATH_LOG "/log/log.txt"
#define FILE_PATH_EX "/example.txt"
#define FILE_OBJ_LOG fil_log
#define FILE_OBJ_EX fil_ex

#define FATFS_BUFF 32
#define NaN FR_DEFAULT

// RELAY
#define RELAY_GPIO_InitStruct GPIO_RELAY_InitStruct

//HC05
#define HC05_BAUDRATE_AT                38400 
#define HC05_BAUDRATE_DATA              38400 
#define HC05_HARD_FLW_CTL               USART_HardwareFlowControl_None
#define HC05_DATA_SIZE                  USART_WordLength_8b
#define HC05_MODE                       USART_Mode_Tx | USART_Mode_Rx
#define HC05_PARITY                     USART_Parity_No
#define HC05_STOP_BITS                  USART_StopBits_1


// USB
#define USB_BAUDRATE                    9600
#define USB_HARD_FLW_CTL                USART_HardwareFlowControl_None
#define USB_DATA_SIZE                   USART_WordLength_8b
#define USB_MODE                        USART_Mode_Tx | USART_Mode_Rx
#define USB_PARITY                      USART_Parity_No
#define USB_STOP_BITS                   USART_StopBits_1
#define USB_BAUD_PRESCALER              SPI_BaudRatePrescaler_4

// GSM
#define _SIM80X_DEBUG 0 //	0: No DEBUG 1:High Level Debug .Use printf 2:All RX Data.Use printf

#define _SIM80X_USE_POWER_KEY 0
#define _SIM80X_BUFFER_SIZE 64
#define _SIM80X_DMA_TRANSMIT 0
#define _SIM80X_USE_BLUETOOTH 1
#define _SIM80X_USE_GPRS 1

#define GSM_BAUDRATE                    9600
#define GSM_HARD_FLW_CTL                USART_HardwareFlowControl_None
#define GSM_DATA_SIZE                   USART_WordLength_8b
#define GSM_MODE                        USART_Mode_Tx | USART_Mode_Rx
#define GSM_PARITY                      USART_Parity_No
#define GSM_STOP_BITS                   USART_StopBits_1

// LCD SETTINGS
#define SPI_LCD_DIRECTION SPI_Direction_2Lines_FullDuplex
#define SPI_LCD_MODE SPI_Mode_Master
#define SPI_LCD_DATA_SIZE SPI_DataSize_8b
#define SPI_LCD_CPOL SPI_CPOL_Low
#define SPI_LCD_CPHA SPI_CPHA_1Edge
#define SPI_LCD_NSS SPI_NSS_Soft | SPI_NSSInternalSoft_Set
#define SPI_LCD_BAUD_PRESCALER SPI_BaudRatePrescaler_2 // APB = 8MHZ, Hence 8/2 = 4MBPS
#define SPI_LCD_FIRSTBIT SPI_FirstBit_MSB

#define LCD_NVIC_CHAN_GDO0 EXTI9_5_IRQn
#define LCD_NVIC_PREPRIOR_GDO0 0x00
#define LCD_NVIC_SUBPRIOR_GDO0 0x00

// W25Q FLASH SETTINGS
#define SPI_SFLASH_DIRECTION SPI_Direction_2Lines_FullDuplex
#define SPI_SFLASH_MODE SPI_Mode_Master
#define SPI_SFLASH_DATA_SIZE SPI_DataSize_8b
#define SPI_SFLASH_CPOL SPI_CPOL_Low
#define SPI_SFLASH_CPHA SPI_CPHA_1Edge
#define SPI_SFLASH_NSS SPI_NSS_Soft | SPI_NSSInternalSoft_Set // SPI_NSS_Soft
#define SPI_SFLASH_BAUD_PRESCALER SPI_BaudRatePrescaler_2     // APB = 8MHZ, Hence 8/2 = 4MBPS
#define SPI_SFLASH_FIRSTBIT SPI_FirstBit_MSB

// PCF8574A SETTINGS
#define I2C_PCF8574_CLK_SPEED				100000			// 100KHZ
#define I2C_PCF8574_MODE                    I2C_Mode_I2C
#define I2C_PCF8574_DUTY_CYCLE				I2C_DutyCycle_2
#define I2C_PCF8574_OWN_ADDR				0x21			// MASTER ADDRESS 7
#define I2C_PCF8574_ACK					    I2C_Ack_Disable
#define I2C_PCF8574_ACK_ADDR				I2C_AcknowledgedAddress_7bit  

//LORA
#define LORA_BAUDRATE                   9600 
#define LORA_HARD_FLW_CTL               USART_HardwareFlowControl_None
#define LORA_DATA_SIZE                  USART_WordLength_8b
#define LORA_MODE                       USART_Mode_Tx | USART_Mode_Rx
#define LORA_PARITY                     USART_Parity_No
#define LORA_STOP_BITS                  USART_StopBits_1


// INTERRUPT ROUTINES
#define CC1101_IRQHANDLER void EXTI9_5_IRQHandler(void)
#define GSM_IRQHANDLER void USART3_IRQHandler(void)

#endif