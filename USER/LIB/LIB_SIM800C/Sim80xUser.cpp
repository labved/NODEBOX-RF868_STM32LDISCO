#include "stm32l1xx.h"  
#include "Settings.h"

#include "HAL_SIM800C.h"
#include "LIB_SIM800C.h"

#ifdef LIB_SIM800C_H

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

extern HAL_SIM800C              sim800c;
extern Sim80x_t                 Sim80x;

void  Sim80x_UserInit(void)
{
  //GPRS_ConnectToNetwork("mcinet","","",false);
  //GPRS_HttpGet("www.google.com");  
}


#endif