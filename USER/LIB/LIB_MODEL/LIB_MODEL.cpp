#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_GSM
  #define _USE_GSM
#endif

#if(_USE_GSM==1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_MODEL.h"
#include "LIB_MODEL.h"

HAL_MODEL                model;
extern Sim80x_t          Sim80x;

//osThreadId    Sim80xTaskHandle;       // RTOS
//osThreadId    Sim80xBuffTaskHandle;   // RTOS

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :Init
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_MODEL::Init(void)  
{
  model.gpioInit();
  model.uartInit();
}

/****************************************************************
*FUNCTION NAME:test
*FUNCTION     :test
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_MODEL::test(void)  
{
  char temp[30];
  
  model.sendAtCommand("AT+GSN=?\r\n",200,2,"AT+GSN=?\r\n", "\r\n+GSN\r\n");                     //sim800c
  model.sendAtCommand("AT+VERSION?\r\n",200,2,"AT+VERSION?\r\n", "+VERSION:", "\r\nOK\r\n");   //hc05
}

extern LIB_MODEL gen;

#endif