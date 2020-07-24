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


//####################################################################################################
bool  LIB_SIM800C::startupNetGPRS(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CIICR\r\n",85000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  if(answer == 1)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nstartupNetGPRS() ---> OK\r\n");
    #endif
    return true;
  }
  else  
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nstartupNetGPRS() --->ERROR\r\n");
    #endif
    return false;
  }
}  
//####################################################################################################
bool  LIB_SIM800C::getNetAPN(char *Name,char *username,char *password)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CSTT?\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  if(answer==1)
  {
    if(Name!=NULL)
      strcpy(Name,Sim80x.GPRS.APN);
    if(username!=NULL)
      strcpy(username,Sim80x.GPRS.APN_UserName);
    if(password!=NULL)
      strcpy(password,Sim80x.GPRS.APN_Password);
    #if (_SIM80X_DEBUG==1)
    printf("\r\ngetNetAPN(%s,%s,%s) <--- OK\r\n",Sim80x.GPRS.APN,Sim80x.GPRS.APN_UserName,Sim80x.GPRS.APN_Password);
    #endif
    return true;
  }
  else
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\ngetNetAPN() <--- ERROR\r\n");
    #endif
    return false;
  }
}
//####################################################################################################
bool  LIB_SIM800C::setNetAPN(char *Name,char *username,char *password)
{
  char str[64];
  uint8_t answer;
  sprintf(str,"AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n",Name,username,password);
  answer = sim800c.sendAtCommand(str,1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  if(answer == 1)
  {
    strcpy(Sim80x.GPRS.APN,Name);
    strcpy(Sim80x.GPRS.APN_UserName,username);
    strcpy(Sim80x.GPRS.APN_Password,password);
    #if (_SIM80X_DEBUG==1)
    printf("\r\nsetNetAPN(\"%s\",\"%s\",\"%s\") ---> OK\r\n",Name,username,password);
    #endif    
    return true;
  }
  else
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nsetNetAPN(\"%s\",\"%s\",\"%s\") ---> ERROR\r\n",Name,username,password);
    #endif    
    return false;  
  }  
}
//####################################################################################################
bool  LIB_SIM800C::deactivateNetPDPContext(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CIPSHUT\r\n",65000,2,"\r\nSHUT OK\r\n","\r\nERROR\r\n");
  if(answer == 1)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\ndeactivateNetPDPContext() ---> OK\r\n");
    #endif    
    return true;
  }
  else
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\ndeactivateNetPDPContext() ---> ERROR\r\n");
    #endif    
    return false;  
  }
}
//####################################################################################################
void  LIB_SIM800C::getNetLocalIP(char *IP)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CIFSR\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  if((IP!=NULL) && (answer==1))
    strcpy(IP,Sim80x.GPRS.LocalIP);
  #if (_SIM80X_DEBUG==1)
  printf("\r\ngetNetLocalIP(%s) <--- OK\r\n",Sim80x.GPRS.LocalIP);
  #endif    
}
//####################################################################################################
void  LIB_SIM800C::getNetCurrentConnectionSts(void)
{
  sim800c.sendAtCommand(" AT+CIPSTATUS\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
  #if (_SIM80X_DEBUG==1)
  printf("\r\ngetNetCurrentConnectionSts() <--- OK\r\n");
  #endif      
}
//####################################################################################################
bool  LIB_SIM800C::getNetMultiConnection(void)
{
  sim800c.sendAtCommand(" AT+CIPMUX?\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
  #if (_SIM80X_DEBUG==1)
  printf("\r\ngetNetMultiConnection(%d) <--- OK\r\n",Sim80x.GPRS.MultiConnection);
  #endif  
  return Sim80x.GPRS.MultiConnection;
}
//####################################################################################################
bool  LIB_SIM800C::setNetMultiConnection(bool Enable)
{
  uint8_t answer;
  if(Enable==true)
    answer = sim800c.sendAtCommand(" AT+CIPMUX=1\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
  else
    answer = sim800c.sendAtCommand(" AT+CIPMUX=0\r\n",1000,2,"\r\nOK\r\n","\r\nERROR\r\n");  
  if(answer == 1)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nsetNetMultiConnection(%d) ---> OK\r\n",Enable);
    #endif  
    Sim80x.GPRS.MultiConnection=Enable;
    return true;
  }
  else
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nsetNetMultiConnection(%d) ---> ERROR\r\n",Enable);
    #endif  
    return false;
  }
}
//####################################################################################################
//####################################################################################################
//####################################################################################################
bool  LIB_SIM800C::GPRS_connectToNetwork(char *Name,char *username,char *password,bool EnableMultiConnection)
{
  
  if(deactivateNetPDPContext()==false)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nGPRS_ConnectToNetwork() ---> ERROR\r\n");
    #endif 
    return false;
  }
  setNetMultiConnection(EnableMultiConnection);
  if(setNetAPN(Name,username,password)==false)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nGPRS_ConnectToNetwork() ---> ERROR\r\n");
    #endif 
    return false;
  }
  if(startupNetGPRS()==false)
  {
    #if (_SIM80X_DEBUG==1)
    printf("\r\nGPRS_ConnectToNetwork() ---> ERROR\r\n");
    #endif 
    return false;
  }
  getNetLocalIP(NULL);  
  #if (_SIM80X_DEBUG==1)
  printf("\r\nGPRS_ConnectToNetwork() ---> OK\r\n");
  #endif 
  return true;
}
//####################################################################################################
bool  LIB_SIM800C::getHttp(char *URL)
{
  uint16_t timeout;
  uint8_t answer;
  char str[100];
  answer = sim800c.sendAtCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n",1000,1,"\r\nOK\r\n");
  if(answer!=1)
    goto Error;
  answer = sim800c.sendAtCommand("AT+SAPBR=1,1\r\n",10000,1,"\r\nOK\r\n");
  if(answer!=1)
    goto Error;
  answer = sim800c.sendAtCommand("AT+HTTPINIT\r\n",1000,1,"\r\nOK\r\n");
  if(answer!=1)
    goto Error;
  answer = sim800c.sendAtCommand("AT+HTTPPARA=\"CID\",1\r\n",1000,1,"\r\nOK\r\n");
  if(answer!=1)
    goto Error;
  
  snprintf(str,sizeof(str),"AT+HTTPPARA=\"URL\",\"%s\"\r\n",URL);
  answer = sim800c.sendAtCommand(str,10000,2,"\r\nOK\r\n","\r\n+CME ERROR");
  if(answer!=1)
    goto Error;
  
  Sim80x.GPRS.HttpAction.ResultCode=0;
  answer = sim800c.sendAtCommand("AT+HTTPACTION=0\r\n",1000,1,"\r\nOK\r\n");
  if(answer!=1)
    goto Error;
  timeout=0;
  while(Sim80x.GPRS.HttpAction.ResultCode==0)
  {
    delay_ms(1000); 
    timeout++;  
    if(timeout == 120)
      goto Error;    
  }
  if(Sim80x.GPRS.HttpAction.ResultCode==200)
  {
    Sim80x.GPRS.HttpAction.TransferStartAddress=0;    
    for(uint32_t start=0 ;start<Sim80x.GPRS.HttpAction.DataLen ; start+=256 )
    {
      Sim80x.GPRS.HttpAction.CopyToBuffer=0;
      sprintf(str,"AT+HTTPREAD=%d,%d\r\n",start,256);
      sim800c.writeString(str);
      timeout=0;
      while(Sim80x.GPRS.HttpAction.CopyToBuffer==0)
      {
        delay_ms(10);
        timeout++;
        if(timeout == 100)
          goto Error;
      } 
      Sim80x.GPRS.HttpAction.TransferStartAddress =  start;
      GPRS_userHttpGetAnswer(Sim80x.GPRS.HttpAction.Data,Sim80x.GPRS.HttpAction.TransferStartAddress,Sim80x.GPRS.HttpAction.TransferDataLen);        
    }    
    answer = sim800c.sendAtCommand("AT+HTTPTERM\r\n",1000,1,"\r\nOK\r\n");
    answer = sim800c.sendAtCommand("AT+SAPBR=0,1\r\n",1000,1,"\r\nOK\r\n");
    #if (_SIM80X_DEBUG==1)
    printf("\r\nGPRS_httpGet(%s) ---> OK\r\n",URL);
    #endif 
    return true;
  }
  Error:
  answer = sim800c.sendAtCommand("AT+HTTPTERM\r\n",1000,1,"\r\nOK\r\n");
  answer = sim800c.sendAtCommand("AT+SAPBR=0,1\r\n",1000,1,"\r\nOK\r\n");
  #if (_SIM80X_DEBUG==1)
  printf("\r\nGPRS_httpGet(%s) ---> ERROR\r\n",URL);
  #endif 
  return false;  
}
//####################################################################################################
#endif
#endif
#endif
