#include "stm32l1xx.h"  
#include "Settings.h"


#ifndef HAL_IOEXP_H
  #define HAL_IOEXP_H

#ifndef _USE_IOEXP
  #define _USE_IOEXP
#endif

#if(_USE_IOEXP==1)

// ADDRESS
#define PCF8574_DEV0_ADDR					0x00
#define PCF8574_DEV1_ADDR					0x01
#define PCF8574_DEV2_ADDR					0x02
#define PCF8574_DEV3_ADDR					0x03
#define PCF8574_DEV4_ADDR					0x04
#define PCF8574_DEV5_ADDR					0x05
#define PCF8574_DEV6_ADDR					0x06
#define PCF8574_DEV7_ADDR					0x07

// HW PINS
#define PCF8574_DEV0_GPIOA_PIN0				0x00
#define PCF8574_DEV0_GPIOA_PIN1				0x01
#define PCF8574_DEV0_GPIOA_PIN2				0x02
#define PCF8574_DEV0_GPIOA_PIN3				0x03
#define PCF8574_DEV0_GPIOA_PIN4				0x04
#define PCF8574_DEV0_GPIOA_PIN5				0x05
#define PCF8574_DEV0_GPIOA_PIN6				0x06
#define PCF8574_DEV0_GPIOA_PIN7				0x07

#define PCF8574_DEV0_GPIOA_PIN0				0x00
#define PCF8574_DEV0_GPIOA_PIN1				0x01
#define PCF8574_DEV0_GPIOA_PIN2				0x02
#define PCF8574_DEV0_GPIOA_PIN3				0x03
#define PCF8574_DEV0_GPIOA_PIN4				0x04
#define PCF8574_DEV0_GPIOA_PIN5				0x05
#define PCF8574_DEV0_GPIOA_PIN6				0x06
#define PCF8574_DEV0_GPIOA_PIN7				0x07

// MASK
#define PCF8574_MASK_PIN0					0x01  
#define PCF8574_MASK_PIN1					0x02  
#define PCF8574_MASK_PIN2					0x04  
#define PCF8574_MASK_PIN3					0x08  
#define PCF8574_MASK_PIN4					0x10  
#define PCF8574_MASK_PIN5					0x20  
#define PCF8574_MASK_PIN6					0x40  
#define PCF8574_MASK_PIN7					0x80

// MASK COUNT
#define PCF8574_MASK_COUNT_PIN0				0 
#define PCF8574_MASK_COUNT_PIN1				1  
#define PCF8574_MASK_COUNT_PIN2				2  
#define PCF8574_MASK_COUNT_PIN3				3  
#define PCF8574_MASK_COUNT_PIN4				4  
#define PCF8574_MASK_COUNT_PIN5				5  
#define PCF8574_MASK_COUNT_PIN6				6  
#define PCF8574_MASK_COUNT_PIN7				7


#endif

#endif
