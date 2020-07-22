#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_SIM800C.h"
#include "LIB_SIM800C.h"

#ifdef LIB_SIM800C_H

#if(_USE_GSM==1)

extern HAL_SIM800C              sim800c;
extern Sim80x_t                 Sim80x;


//##################################################################################################################
void  LIB_SIM800C::Gsm_user(uint32_t StartupTime)
{
  
  
}
//##################################################################################################################
void  LIB_SIM800C::Gsm_userNewCall(const char *CallerNumber)
{
  //Gsm_CallDisconnect(); 
  //Gsm_CallAnswer();    

}
//##################################################################################################################
void  LIB_SIM800C::Gsm_userNewMsg(char *Number,char *Date,char *Time,char *msg)
{
  
  
  
}
//##################################################################################################################
#endif

#endif