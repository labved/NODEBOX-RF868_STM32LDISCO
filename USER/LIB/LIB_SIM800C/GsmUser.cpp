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


/****************************************************************
*FUNCTION NAME:Gsm_user
*FUNCTION     :Gsm_user
*INPUT        :StartupTime
*OUTPUT       :void
****************************************************************/
void  LIB_SIM800C::smsUser(uint32_t StartupTime)
{
  
  
}

/****************************************************************
*FUNCTION NAME:Gsm_userNewMsg
*FUNCTION     :Gsm_userNewMsg
*INPUT        :Number, Date, Time, kmsg
*OUTPUT       :void
****************************************************************/
void  LIB_SIM800C::smsUserNewMsg(char *Number,char *Date,char *Time,char *msg)
{
  
  
  
}
//##################################################################################################################
#endif

#endif