#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_LORA
#define _USE_LORA
#endif

#if (_USE_LORA == 1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"
#include "Functions.h"
#include "Conversions.h"

#include "HAL_E70-868T30S.h"
#include "LIB_E70-868T30S.h"

HAL_E70_868T30S rf868;

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :HC05 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_E70_868T30S::Init()
{
  rf868.uartInit();
  PoweronReset();

  RegConfigSettings();
}
/****************************************************************
*FUNCTION NAME:PoweronReset
*FUNCTION     :Switch reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_E70_868T30S::PoweronReset(void)
{
    
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :HC05 register config //details refer datasheet of ST7586S//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_E70_868T30S::RegConfigSettings(void)
{
/* steps to follow
1. set the tx 
config mode
send clora commands 
wait for aux to raising edge
set the mode to powersaver mode
switch off the tx rx*/
  rf868.setRfTx();
  rf868.setConfigurationMode();
  rf868.writeString("");
  rf868.setPowerSavingMode();
  rf868.resetRfTxRx();

}


/****************************************************************
*FUNCTION NAME:setATMode
*FUNCTION     :set device to respond to AT Command Mode
*INPUT        :none
*OUTPUT       :none
****************************************************************/
 void LIB_E70_868T30S::setAtMode(void)
 {
   rf868.uartInit();
   
   // GPIO_ResetBits(HC05_GPIO_PWR, HC05_PIN_PWR);      //POWER OFF
   // GPIO_SetBits(HC05_GPIO_KEY, HC05_PIN_KEY);      //  KEY PIN HIGH TO ON COMMAND AT MODE
   //  
   // delay_ms(300);
   //  
   // GPIO_SetBits(HC05_GPIO_PWR, HC05_PIN_PWR);     //POWER ON
   
 }
 /****************************************************************
*FUNCTION NAME:setDataMode
*FUNCTION     :set device to respond to Data Mode
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_E70_868T30S::setDataMode(void)
{
     
}

/****************************************************************
*FUNCTION NAME:softReset
*FUNCTION     :Reset on command
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_E70_868T30S::softReset(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(reset_cmd, "RESET");
  
  return 1;
}


/****************************************************************
*FUNCTION NAME:testCommand
*FUNCTION     :HC05 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_E70_868T30S::testCommand(void)
{
}


extern LIB_E70_868T30S lora;

#endif


   
