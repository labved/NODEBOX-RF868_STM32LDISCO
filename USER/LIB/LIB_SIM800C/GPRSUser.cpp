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

#if (_SIM80X_USE_GPRS==1)


//#########################################################################################################
void     LIB_SIM800C::GPRS_userHttpGetAnswer(char *data,uint32_t StartAddress,uint16_t dataLen)
{
  
  
}
//#########################################################################################################


#endif
#endif
#endif
