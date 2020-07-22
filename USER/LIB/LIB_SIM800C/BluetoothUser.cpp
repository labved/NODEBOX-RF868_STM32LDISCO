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

#if (_SIM80X_USE_BLUETOOTH==1)
//###################################################################################################################
//I dont know why. not work yet . I set auto pair.


/****************************************************************
*FUNCTION NAME:Bluetooth_UserNewPairingRequest
*FUNCTION     :Bluetooth_UserNewPairingRequest
*INPUT        :Name,Address,Pass; //refer defines in header file
*OUTPUT       :none
****************************************************************/

void   LIB_SIM800C::Bluetooth_userNewPairingRequest(char *Name,char *Address,char *Pass)
{
  //Bluetooth_acceptPair(true);  
  //Bluetooth_acceptPair(false);
}
//###################################################################################################################

/****************************************************************
*FUNCTION NAME:Bluetooth_UserConnectingSpp
*FUNCTION     :Bluetooth_UserConnectingSpp
*INPUT        :none; //refer defines in header file
*OUTPUT       :none
****************************************************************/

void  LIB_SIM800C::Bluetooth_userConnectingSpp(void)
{
  Bluetooth_sppAllowConnection(true);  
  //Bluetooth_sppAllowConnection(false);
}
//###################################################################################################################

/****************************************************************
*FUNCTION NAME:Bluetooth_UserNewSppData
*FUNCTION     :Bluetooth_UserNewSppData
*INPUT        :NewData, len; //refer defines in header file
*OUTPUT       :none
****************************************************************/

void  LIB_SIM800C::Bluetooth_userNewSppData(char *NewData,uint16_t len)
{
  //Bluetooth_sppSend("Test Back\r\n");  
}
//###################################################################################################################


#endif
#endif
#endif
