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

/****************************************************************
*FUNCTION NAME:getBtHostName
*FUNCTION     :getBtHostName
*INPUT        :void //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::getBtHostName(void)
{
  uint8_t answer;
  
  memset(Sim80x.Bluetooth.HostName,0,sizeof(Sim80x.Bluetooth.HostName));
  memset(Sim80x.Bluetooth.HostAddress,0,sizeof(Sim80x.Bluetooth.HostAddress));
  
  answer = sim800c.sendAtCommand("AT+BTHOST?\r\n",1000,1,"\r\n+BTHOST:");
   
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_getBtHostName");

  if((answer == 1) && (Sim80x.Bluetooth.HostName[0] != 0) && (Sim80x.Bluetooth.HostAddress[0] != 0))
    return true;
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:getBtStatus
*FUNCTION     :getBtStatus
*INPUT        :void  //refer defines in header file
*OUTPUT       :BluetoothStatus_t
****************************************************************/
BluetoothStatus_t  LIB_SIM800C::getBtStatus(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+BTSTATUS?\r\n",1000,1,"\r\n+BTSTATUS:");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_getBtStatus");

  if(answer == 1)
    return Sim80x.Bluetooth.Status;
  else
    return BluetoothStatus_Error;
}

/****************************************************************
*FUNCTION NAME:getBtVisibility
*FUNCTION     :getBtVisibility
*INPUT        :void //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::getBtVisibility(void)
{  
  sim800c.sendAtCommand("AT+BTVIS?\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  debugTerminal("Sim80x_getBtVisibility");

  return Sim80x.Bluetooth.Visibility;
}

/****************************************************************
*FUNCTION NAME:setBtPower
*FUNCTION     :eurn on bluetooth
*INPUT        :TurnOn
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::setBtPower(bool TurnOn)
{
  uint8_t answer;
  delay_ms(100);
  getBtStatus();
  if(TurnOn == true)
  {
    if(Sim80x.Bluetooth.Status == BluetoothStatus_Initial)
    {
      answer = sim800c.sendAtCommand("AT+BTPOWER=1\r\n",5000,2,"\r\nOK\r\n","\r\nERROR\r\n");
     
      memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

      debugTerminal("Sim80x_setBtPower");

    if(answer == 1)
        {
          for(uint8_t i=0 ;i<50 ;i++)
          {
            delay_ms(100);
            if(getBtStatus()>BluetoothStatus_Initial)
            {
              getBtStatus();
              return true;          
            }
          }        
        }      
      }
      else if(Sim80x.Bluetooth.Status == BluetoothStatus_Error)
      {
        return false;
      }
      else
      {
        getBtStatus();
        return true;       
      }
    }
    else
    {
      for(uint8_t i=0 ;i<50 ;i++)
      {
        delay_ms(100);

        answer = sim800c.sendAtCommand("AT+BTPOWER=0\r\n",5000,2,"\r\nOK\r\n","\r\nERROR\r\n");

        if(getBtStatus()==BluetoothStatus_Initial)
        {
          getBtStatus();
          return true;      
        }
      }
      return false;     
    }
    return false; 
}

/****************************************************************
*FUNCTION NAME:setBtHostName
*FUNCTION     :setBtHostName
*INPUT        :HostName //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::setBtHostName(char *HostName)
{
  uint8_t answer;
  char  str[32];
  char  strParam[32];
  
  snprintf(str,sizeof(str),"AT+BTHOST=%s\r\n",HostName);
  snprintf(strParam,sizeof(strParam),"AT+BTHOST=%s\r\r\nOK\r\n",HostName);
  
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_setBtHostName");
  if(answer == 1)
  {
    getBtHostName();
    return true;
  }
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:setBtAutoPair
*FUNCTION     :setBtAutoPair
*INPUT        :Enable //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setBtAutoPair(bool  Enable)
{
  uint8_t answer;
  if(Enable==true)
    answer = sim800c.sendAtCommand("AT+BTPAIRCFG=2\r\n",1000,2,"AT+BTPAIRCFG=2\r\r\nOK\r\n","AT+BTPAIRCFG=2\r\r\nERROR\r\n");
  else
    answer = sim800c.sendAtCommand("AT+BTPAIRCFG=0\r\n",1000,2,"AT+BTPAIRCFG=2\r\r\nOK\r\n","AT+BTPAIRCFG=2\r\r\nERROR\r\n");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_setBtAutoPair");
  if(answer == 1)
    return true;
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:setBtPairPassword
*FUNCTION     :setBtPairPassword
*INPUT        :Pass //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setBtPairPassword(char  *Pass)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+BTPAIRCFG=1,%s\r\n",Pass);
  snprintf(strParam,sizeof(strParam),"AT+BTPAIRCFG=1,%s\r\r\nOK\r\n",Pass);
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_setBtPairPassword");
  if(answer == 1)
    return true;
  else
    return false;    
}

/****************************************************************
*FUNCTION NAME:setBtVisibility
*FUNCTION     :setBtVisibility
*INPUT        :Visible //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::setBtVisibility(bool Visible)//what should i do here
{
  uint8_t answer;
  char str[16];
  snprintf(str,sizeof(str),"AT+BTVIS=%d\r\n",Visible);
  answer = sim800c.sendAtCommand(str,1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_setBtVisibility");
  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:acceptPair
*FUNCTION     :acceptPair
*INPUT        :Accept //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::acceptPair(bool Accept)  
{
  uint8_t answer;
  if(Accept == true)
  {
    answer = sim800c.sendAtCommand("AT+BTPAIR:1,1\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
   
    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

    debugTerminal("Sim80x_acceptPair");
  if(answer == 1)
      return true;
    else
      return false;
  }
  else
  {
    answer = sim800c.sendAtCommand("AT+BTPAIR:1,0\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
   
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_");
  if(answer == 1)
      return true;
    else
      return false;    
  }  
}

/****************************************************************
*FUNCTION NAME:acceptPairWithPass
*FUNCTION     :acceptPairWithPass
*INPUT        :Pass //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::acceptPairWithPass(char *Pass)  
{
  uint8_t answer;
  char str[32];
  snprintf(str,sizeof(str),"AT+BTPAIR:2,%s\r\n",Pass);
  
  answer = sim800c.sendAtCommand(str,1000,1,"\r\nOK\r\n");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_acceptPairWithPass");
  if(answer == 1)
    return true;
  else
    return false;
  
}

/****************************************************************
*FUNCTION NAME:btUnpair
*FUNCTION     :btUnpair
*INPUT        :Unpair_0_to_all //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::btUnpair(uint8_t  Unpair_0_to_all)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+BTUNPAIR=%d\r\n",Unpair_0_to_all);
  snprintf(strParam,sizeof(strParam),"AT+BTUNPAIR=%d\r\n",Unpair_0_to_all);
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_btUnpair");
  if(answer == 1)
    return true;
  else
    return false;    
}

/****************************************************************
*FUNCTION NAME:btSppAllowConnection
*FUNCTION     :btSppAllowConnection
*INPUT        :Accept //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::btSppAllowConnection(bool Accept)//what should i do here
{
  uint8_t answer;
  char str[16];
  snprintf(str,sizeof(str),"AT+BTACPT=%d\r\n",Accept);
  answer = sim800c.sendAtCommand(str,1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
  
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_btSppAllowConnection");
  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:btSppSend
*FUNCTION     :btSppSend
*INPUT        :DataString //refer defines in header file
*OUTPUT       :bool
****************************************************************/
bool  LIB_SIM800C::btSppSend(char *DataString)
{
  uint8_t answer;
  char str[2];
  answer = sim800c.sendAtCommand("AT+BTSPPSEND\r\n",1000,2,"\r\r\n> ","\r\nERROR\r\n");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_btSppSend");
  
  if(answer == 1)
  {
    sim800c.writeString(DataString);
    sprintf(str,"%c",26);
    answer = sim800c.sendAtCommand(str,1000,2,"\r\nSEND OK\r\n","\r\nERROR\r\n");
   
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));  

  debugTerminal("Sim80x_btSppSend");
  if(answer == 1)
      return true;
    else
      return false;
  }
  else
    return false;
}
//#################################################################################################################





#endif
#endif
#endif
