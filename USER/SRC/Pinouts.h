#include "stm32l1xx.h"

#ifndef PINOUTS_H
  #define PINOUTS_H

// WSN - PINOUTS
#define LED_AHB             RCC_AHBPeriph_GPIOB
#define LED_APB             RCC_APB2Periph_SYSCFG

#define LED_GPIO            GPIOB
#define LEDON_PIN           GPIO_Pin_7

#define LED_GPIO            GPIOB
#define LED_COM_PIN         GPIO_Pin_8

// MCO
#define MCO_AHB             RCC_AHBPeriph_GPIOA
#define MCO_GPIO            GPIOA
#define MCO_PIN             GPIO_Pin_8

// RTC
#define RTC_GPIO_AHB RCC_AHBPeriph_GPIOC
#define RTC_APB_PWR RCC_APB1Periph_PWR

#define RTC_GPIO_OUT GPIOC
#define RTC_PIN_OUT GPIO_Pin_13

#define RTC_GPIO_AF GPIO_AF_RTC_AF1
#define RTC_AF_PIN_OUT GPIO_PinSource13

#define RTC_EXTI_GPIO_OUT EXTI_PortSourceGPIOC
#define RTC_EXTI_LINE_OUT EXTI_Line13

// HC05 UART - PINOUTS
/**********************UART*************************/
#define HC05_RCC_TX RCC_AHBPeriph_GPIOA
#define HC05_RCC_RX RCC_AHBPeriph_GPIOA

#define HC05_RCC_APB_UART RCC_APB2Periph_USART1 

#define HC05_USART_CH USART1        
#define HC05_GPIO_AF GPIO_AF_USART1 

#define HC05_GPIO_TX       GPIOA       
#define HC05_PIN_TX GPIO_Pin_9 

#define HC05_GPIO_RX       GPIOA       
#define HC05_PIN_RX GPIO_Pin_10 

#define HC05_AF_PIN_TX GPIO_PinSource9 
#define HC05_AF_PIN_RX GPIO_PinSource10 

/**********************STATE*************************/
#define HC05_RCC_STATE RCC_AHBPeriph_GPIOC
#define HC05_STATE_SYSCONFIG_APB  RCC_APB2Periph_SYSCFG 

#define HC05_GPIO_STATE GPIOC       
#define HC05_PIN_STATE GPIO_Pin_3

/**********************EN*************************/
#define HC05_RCC_EN                             RCC_AHBPeriph_GPIOC
#define HC05_EN_SYSCONFIG_APB                   RCC_APB2Periph_SYSCFG 

#define HC05_GPIO_EN                            GPIOC       
#define HC05_PIN_EN                             GPIO_Pin_2

/**********************KEY*************************/
#define HC05_RCC_KEY                            RCC_AHBPeriph_GPIOC
#define HC05_KEY_SYSCONFIG_APB                  RCC_APB2Periph_SYSCFG 

#define HC05_GPIO_KEY                           GPIOC       
#define HC05_PIN_KEY                            GPIO_Pin_1

// LCD SPI - PINOUTS
/**********************SPI*************************/
#define LCD_RCC_GPIO RCC_AHBPeriph_GPIOB
#define LCD_RCC_APB RCC_APB1Periph_SPI2
#define LCD_GPIO_AF GPIO_AF_SPI2
#define LCD_SPI_CH SPI2

/**********************MOSI, MISO, CLK**************/
#define LCD_GPIO_SPI GPIOB

#define LCD_PIN_MOSI GPIO_Pin_15
#define LCD_PIN_MISO GPIO_Pin_14
#define LCD_PIN_CLK GPIO_Pin_13

#define LCD_AF_PIN_MOSI GPIO_PinSource15
#define LCD_AF_PIN_MISO GPIO_PinSource14
#define LCD_AF_PIN_CLK GPIO_PinSource13

/**********************NSS*************************/
#define LCD_RCC_GPIO_NSS RCC_AHBPeriph_GPIOB
#define LCD_NSS_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define LCD_GPIO_NSS GPIOB
#define LCD_PIN_NSS GPIO_Pin_12

/**********************RST*************************/
#define LCD_RCC_RST RCC_AHBPeriph_GPIOC
#define LCD_RST_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define LCD_GPIO_RST GPIOC
#define LCD_PIN_RST GPIO_Pin_8

/**********************A0*************************/
#define LCD_RCC_A0 RCC_AHBPeriph_GPIOC

#define LCD_A0_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define LCD_GPIO_A0 GPIOC
#define LCD_PIN_A0 GPIO_Pin_9

// SD CARD SPI - PINOUTS
/**********************SPI*************************/
#define SDMMC_RCC_GPIO RCC_AHBPeriph_GPIOB
#define SDMMC_RCC_APB RCC_APB1Periph_SPI2

#define SDMMC_GPIO_AF GPIO_AF_SPI2
#define SDMMC_SPI_CH SPI2

/**********************MOSI, MISO, CLK**************/
#define SDMMC_GPIO_SPI GPIOB

#define SDMMC_PIN_MOSI GPIO_Pin_15
#define SDMMC_PIN_MISO GPIO_Pin_14
#define SDMMC_PIN_CLK GPIO_Pin_13

#define SDMMC_AF_PIN_MOSI GPIO_PinSource15
#define SDMMC_AF_PIN_MISO GPIO_PinSource14
#define SDMMC_AF_PIN_CLK GPIO_PinSource13

/**********************NSS*************************/
#define SDMMC_RCC_GPIO_NSS RCC_AHBPeriph_GPIOD
#define SDMMC_RCC_APB_NSS RCC_APB2Periph_SYSCFG

#define SDMMC_GPIO_NSS GPIOD
#define SDMMC_PIN_NSS GPIO_Pin_2

/**********************CD*************************/
#define SDMMC_RCC_CD RCC_AHBPeriph_GPIOC
#define SDMMC_CD_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define SDMMC_GPIO_CD GPIOC
#define SDMMC_PIN_CD GPIO_Pin_12

#define SDMMC_EXTI_GPIO_CD EXTI_PortSourceGPIOC
#define SDMMC_PINSRC_CD EXTI_PinSource12
#define SDMMC_EXTI_LINE_CD EXTI_Line12
#define SDMMC_EXTI_MODE_CD EXTI_Mode_Interrupt
#define SDMMC_EXTI_TYP_CD EXTI_Trigger_Falling



// FLASH SPI - PINOUTS
/**********************SPI*************************/
#define SFLASH_RCC_GPIO RCC_AHBPeriph_GPIOA
#define SFLASH_RCC_APB RCC_APB2Periph_SPI1
#define SPI_SFLASH_GPIO_AF GPIO_AF_SPI1
#define SFLASH_SPI_CH SPI1
/**********************MOSI, MISO, CLK**************/
#define SFLASH_GPIO_SPI GPIOA

#define SFLASH_PIN_MOSI GPIO_Pin_12
#define SFLASH_PIN_MISO GPIO_Pin_11
#define SFLASH_PIN_CLK GPIO_Pin_5

#define SFLASH_AF_PIN_MOSI GPIO_PinSource12
#define SFLASH_AF_PIN_MISO GPIO_PinSource11
#define SFLASH_AF_PIN_CLK GPIO_PinSource5

/**********************NSS*************************/
#define SFLASH_RCC_NSS RCC_AHBPeriph_GPIOA
#define SFLASH_NSS_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define SFLASH_GPIO_NSS GPIOA
#define SFLASH_PIN_NSS GPIO_Pin_4

/**********************WP*************************/
#define SFLASH_RCC_WP RCC_AHBPeriph_GPIOC
#define SFLASH_WP_SYSCONFIG_APB RCC_APB2Periph_SYSCFG

#define SFLASH_GPIO_WP GPIOC
#define SFLASH_PIN_WP GPIO_Pin_6

/**********************HOLD*************************/
#define SFLASH_RCC_HOLD RCC_AHBPeriph_GPIOA
#define SFLASH_HOLD_SYSCONFIG_APB RCC_APB2Periph_SYSCFG
#define SFLASH_GPIO_HOLD GPIOC
#define SFLASH_PIN_HOLD GPIO_Pin_7

// GSM PINOUTS
/**********************UART*************************/
#define GSM_RCC_TX              RCC_AHBPeriph_GPIOA
#define GSM_RCC_RX              RCC_AHBPeriph_GPIOA
#define GSM_RCC_APB             RCC_APB1Periph_USART2 

#define GSM_USART_CH            USART2        
#define GSM_GPIO_AF             GPIO_AF_USART2

#define GSM_GPIO_TX             GPIOA       
#define GSM_PIN_TX              GPIO_Pin_2 
#define GSM_GPIO_RX             GPIOA       
#define GSM_PIN_RX              GPIO_Pin_3 

#define GSM_AF_PIN_TX           GPIO_PinSource2 
#define GSM_AF_PIN_RX           GPIO_PinSource3 

#define CC1101_SYSCONFIG_GDO0   RCC_APB2Periph_SYSCFG   // TO BE CHANGED
#define CC1101_EXTI_GPIO_GDO0   EXTI_PortSourceGPIOC    // TO BE CHANGED
#define CC1101_PINSRC_GDO0      EXTI_PinSource6         // TO BE CHANGED
#define CC1101_EXTI_LINE_GDO0   EXTI_Line6              // TO BE CHANGED
#define CC1101_EXTI_MODE_GDO0   EXTI_Mode_Interrupt     // TO BE CHANGED
#define CC1101_EXTI_TYP_GDO0    EXTI_Trigger_Falling    // TO BE CHANGED

// PWR GPIO
#define GSM_RCC_PWR            RCC_AHBPeriph_GPIOA
#define GSM_PWR_SYSCONFIG_APB  RCC_APB2Periph_SYSCFG 

#define GSM_GPIO_PWR            GPIOA
#define GSM_PIN_PWR             GPIO_Pin_1

// NET GPIO
#define GSM_RCC_NET            RCC_AHBPeriph_GPIOC
#define GSM_NET_SYSCONFIG_APB  RCC_APB2Periph_SYSCFG 

#define GSM_GPIO_NET           GPIOC       
#define GSM_PIN_NET            GPIO_Pin_3

// RST GPIO
#define GSM_RCC_RST             RCC_AHBPeriph_GPIOC
#define GSM_RST_SYSCONFIG_APB   RCC_APB2Periph_SYSCFG 

#define GSM_GPIO_RST            GPIOC       
#define GSM_PIN_RST             GPIO_Pin_0

#define GSM_USART_CH_IRQn       USART1_IRQn

/**********************LORA UART *************************/
// RCC
#define LORA_RCC_TX                     RCC_AHBPeriph_GPIOC
#define LORA_RCC_RX                     RCC_AHBPeriph_GPIOC
#define LORA_RCC_APB                    RCC_APB1Periph_USART3

// UART     
#define LORA_USART_CH                   USART3
#define LORA_GPIO_AF                    GPIO_AF_USART3

#define LORA_GPIO_TX                    GPIOC
#define LORA_PIN_TX                     GPIO_Pin_10

#define LORA_GPIO_RX                    GPIOC
#define LORA_PIN_RX                     GPIO_Pin_11

#define LORA_AF_PIN_TX                  GPIO_PinSource10
#define LORA_AF_PIN_RX                  GPIO_PinSource11

// AUX GPIO     
#define LORA_RCC_AUX                    RCC_AHBPeriph_GPIOC
#define LORA_AUX_SYSCONFIG_APB          RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_AUX                   GPIOC       
#define LORA_PIN_AUX                    GPIO_Pin_0 

#define LORA_USART_CH_IRQn              USART2_IRQn

#define LORA_SYSCONFIG_AUX                          RCC_APB2Periph_SYSCFG   // TO BE CHANGED
#define LORA_EXTI_GPIO_AUX                          EXTI_PortSourceGPIOC    // TO BE CHANGED
#define LORA_PINSRC_AUX                             EXTI_PinSource6         // TO BE CHANGED
#define LORA_EXTI_LINE_AUX                          EXTI_Line6              // TO BE CHANGED
#define LORA_EXTI_MODE_AUX                          EXTI_Mode_Interrupt     // TO BE CHANGED
#define LORA_EXTI_TYP_AUX                           EXTI_Trigger_Falling    // TO BE CHANGED


// M0               
#define LORA_RCC_M0                                 RCC_AHBPeriph_GPIOA
#define LORA_M0_SYSCONFIG_APB                       RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_M0                                GPIOA       
#define LORA_PIN_M0                                 GPIO_Pin_1 

// M1               
#define LORA_RCC_M1                                 RCC_AHBPeriph_GPIOA
#define LORA_M1_SYSCONFIG_APB                       RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_M1                                GPIOA       
#define LORA_PIN_M1                                 GPIO_Pin_2 

// M2               
#define LORA_RCC_M2                                 RCC_AHBPeriph_GPIOA
#define LORA_M2_SYSCONFIG_APB                       RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_M2                                GPIOA       
#define LORA_PIN_M2                                 GPIO_Pin_3           // TO BE CHECKED WHETHER GPIO IS WORKING

// LNA_EN               
#define LORA_RCC_LNAEN                              RCC_AHBPeriph_GPIOC
#define LORA_LNAEN_SYSCONFIG_APB                    RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_LNAEN                             GPIOC       
#define LORA_PIN_LNAEN                              GPIO_Pin_1 

// PA EN 
#define LORA_RCC_PAEN                               RCC_AHBPeriph_GPIOC
#define LORA_PAEN_SYSCONFIG_APB                     RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_PAEN                              GPIOC       
#define LORA_PIN_PAEN                               GPIO_Pin_2 

// RST
#define LORA_RCC_RST                                RCC_AHBPeriph_GPIOC
#define LORA_RST_SYSCONFIG_APB                      RCC_APB2Periph_SYSCFG 

#define LORA_GPIO_RST                               GPIOC       
#define LORA_PIN_RST                                GPIO_Pin_3 

/**********************PCF8574 I2C*************************/
// RCC
#define PCF8574_RCC_SCL                     RCC_AHBPeriph_GPIOB
#define PCF8574_RCC_SDA                     RCC_AHBPeriph_GPIOB
#define PCF8574_RCC_APB_I2C                 RCC_APB1Periph_I2C1

// I2C
#define PCF8574_I2C_CH						I2C1 
#define I2C_PCF8574_GPIO_AF                 GPIO_AF_I2C1
#define I2C_PCF8574_APB_GPIO_I2C			RCC_APB2Periph_GPIOB
#define I2C_PCF8574_APB_MOD_I2C				RCC_APB1Periph_I2C1
#define I2C_PCF8574_APB_AFIO_I2C			RCC_APB2Periph_AFIO

// SCL, SDA
#define PCF8574_GPIO_I2C					GPIOB           
#define PCF8574_PIN_SCL						GPIO_Pin_8
#define PCF8574_PIN_SDA						GPIO_Pin_9

#define PCF8574_AF_PIN_SCL					GPIO_PinSource8
#define PCF8574_AF_PIN_SDA					GPIO_PinSource9

// OTHERS
#define I2C_PCF8574_APB_GPIO_RST			RCC_APB2Periph_GPIOB

#define PCF8574_GPIO_RST					GPIOB
#define PCF8574_PIN_RST						GPIO_Pin_1

#define PCF8574_GPIO_INTA					GPIOB
#define PCF8574_PIN_INTA					GPIO_Pin_1

#define PCF8574_GPIO_INTB					GPIOB
#define PCF8574_PIN_INTB					GPIO_Pin_1


// RELAY PINOUTS
#define RELAY_RCC_AHB RCC_AHBPeriph_GPIOB //Pin not assigned
#define RELAY_GPIO GPIOB                  //Pin not assigned
#define RELAY_PIN_CH1 GPIO_Pin_3          //Pin not assigned
#define RELAY_PIN_CH2 GPIO_Pin_4          //Pin not assigned

#endif
