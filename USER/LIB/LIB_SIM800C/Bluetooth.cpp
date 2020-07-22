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
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_setPower(bool TurnOn)
{
  uint8_t answer;
  delay_ms(100);
  Bluetooth_getStatus();
  if(TurnOn == true)
  {
    if(Sim80x.Bluetooth.Status == BluetoothStatus_Initial)
    {
      answer = sim800c.sendAtCommand("AT+BTPOWER=1\r\n",5000,2,"\r\nOK\r\n","\r\nERROR\r\n");
      if(answer == 1)
      {
        for(uint8_t i=0 ;i<50 ;i++)
        {
          delay_ms(100);
          if(Bluetooth_getStatus()>BluetoothStatus_Initial)
          {
            Bluetooth_getStatus();
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
      Bluetooth_getStatus();
      return true;       
    }
  }
  else
  {
    for(uint8_t i=0 ;i<50 ;i++)
    {
      delay_ms(100);
      answer = sim800c.sendAtCommand("AT+BTPOWER=0\r\n",5000,2,"\r\nOK\r\n","\r\nERROR\r\n");
      if(Bluetooth_getStatus()==BluetoothStatus_Initial)
      {
        Bluetooth_getStatus();
        return true;      
      }
    }
    return false;     
  }
  return false; 
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_getHostName(void)
{
  uint8_t answer;
  memset(Sim80x.Bluetooth.HostName,0,sizeof(Sim80x.Bluetooth.HostName));
  memset(Sim80x.Bluetooth.HostAddress,0,sizeof(Sim80x.Bluetooth.HostAddress));
  answer = sim800c.sendAtCommand("AT+BTHOST?\r\n",1000,1,"\r\n+BTHOST:");
  if((answer == 1) && (Sim80x.Bluetooth.HostName[0] != 0) && (Sim80x.Bluetooth.HostAddress[0] != 0))
    return true;
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_setHostName(char *HostName)
{
  uint8_t answer;
  char  str[32];
  char  strParam[32];
  
  snprintf(str,sizeof(str),"AT+BTHOST=%s\r\n",HostName);
  snprintf(strParam,sizeof(strParam),"AT+BTHOST=%s\r\r\nOK\r\n",HostName);
  
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
  if(answer == 1)
  {
    Bluetooth_getHostName();
    return true;
  }
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
BluetoothStatus_t  LIB_SIM800C::Bluetooth_getStatus(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+BTSTATUS?\r\n",1000,1,"\r\n+BTSTATUS:");
  if(answer == 1)
    return Sim80x.Bluetooth.Status;
  else
    return BluetoothStatus_Error;
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_acceptPair(bool Accept)  
{
  uint8_t answer;
  if(Accept == true)
  {
    answer = sim800c.sendAtCommand("AT+BTPAIR:1,1\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
    if(answer == 1)
      return true;
    else
      return false;
  }
  else
  {
    answer = sim800c.sendAtCommand("AT+BTPAIR:1,0\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
    if(answer == 1)
      return true;
    else
      return false;    
  }  
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_acceptPairWithPass(char *Pass)  
{
  uint8_t answer;
  char str[32];
  snprintf(str,sizeof(str),"AT+BTPAIR:2,%s\r\n",Pass);
  answer = sim800c.sendAtCommand(str,1000,1,"\r\nOK\r\n");
  if(answer == 1)
    return true;
  else
    return false;
  
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool LIB_SIM800C::Bluetooth_setAutoPair(bool  Enable)
{
  uint8_t answer;
  if(Enable==true)
    answer = sim800c.sendAtCommand("AT+BTPAIRCFG=2\r\n",1000,2,"AT+BTPAIRCFG=2\r\r\nOK\r\n","AT+BTPAIRCFG=2\r\r\nERROR\r\n");
  else
    answer = sim800c.sendAtCommand("AT+BTPAIRCFG=0\r\n",1000,2,"AT+BTPAIRCFG=2\r\r\nOK\r\n","AT+BTPAIRCFG=2\r\r\nERROR\r\n");
  if(answer == 1)
    return true;
  else
    return false;
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool LIB_SIM800C::Bluetooth_setPairPassword(char  *Pass)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+BTPAIRCFG=1,%s\r\n",Pass);
  snprintf(strParam,sizeof(strParam),"AT+BTPAIRCFG=1,%s\r\r\nOK\r\n",Pass);
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
  if(answer == 1)
    return true;
  else
    return false;    
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool LIB_SIM800C::Bluetooth_unpair(uint8_t  Unpair_0_to_all)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+BTUNPAIR=%d\r\n",Unpair_0_to_all);
  snprintf(strParam,sizeof(strParam),"AT+BTUNPAIR=%d\r\n",Unpair_0_to_all);
  answer = sim800c.sendAtCommand(str,1000,1,strParam);
  if(answer == 1)
    return true;
  else
    return false;    
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_getVisibility(void)
{
  sim800c.sendAtCommand("AT+BTVIS?\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  return Sim80x.Bluetooth.Visibility;
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void  LIB_SIM800C::Bluetooth_setVisibility(bool Visible)//what should i do here
{
 char str[16];
  snprintf(str,sizeof(str),"AT+BTVIS=%d\r\n",Visible);
  sim800c.sendAtCommand(str,1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void  LIB_SIM800C::Bluetooth_sppAllowConnection(bool Accept)//what should i do here
{
  char str[16];
  snprintf(str,sizeof(str),"AT+BTACPT=%d\r\n",Accept);
  sim800c.sendAtCommand(str,1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :USARTx,UsartRxTemp,state; //refer defines in header file
*OUTPUT       :none
****************************************************************/
bool  LIB_SIM800C::Bluetooth_sppSend(char *DataString)
{
  uint8_t answer;
  char str[2];
  answer = sim800c.sendAtCommand("AT+BTSPPSEND\r\n",1000,2,"\r\r\n> ","\r\nERROR\r\n");
  if(answer == 1)
  {
    sim800c.writeString(DataString);
    sprintf(str,"%c",26);
    answer = sim800c.sendAtCommand(str,1000,2,"\r\nSEND OK\r\n","\r\nERROR\r\n");
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
