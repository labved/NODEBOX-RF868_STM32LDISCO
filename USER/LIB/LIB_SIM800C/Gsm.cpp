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

//######################################################################################################
bool  LIB_SIM800C::Gsm_ussd(char *send,char *receive)
{
  uint8_t answer;
  char str[32];
  
  snprintf(str, sizeof(str), "AT+CUSD=1,\"%s\"\r\n", send);
  memset(Sim80x.Gsm.Msg, 0, sizeof(Sim80x.Gsm.Msg));
  answer = sim800c.sendAtCommand(str, 60000, 2, "\r\n+CUSD:", "\r\n+CME ERROR:");
  if((answer == 1) && (Sim80x.Gsm.Msg[0] != 0) && (receive != NULL))
  {
    memcpy(receive, Sim80x.Gsm.Msg, strlen(Sim80x.Gsm.Msg));
    return true;
  }

  return false;
}
//######################################################################################################
//######################################################################################################
//######################################################################################################
GsmMsgFormat_t  LIB_SIM800C::getSmsFormat(void)
{
  uint8_t   answer;
  answer = sim800c.sendAtCommand("AT+CMGF?\r\n",1000,2,"+CMGF: 0\r\n\r\nOK\r\n","+CMGF: 1\r\n\r\nOK\r\n");
  if(answer == 1)
  {
    Sim80x.Gsm.MsgFormat = GsmMsgFormat_PDU;
    return GsmMsgFormat_PDU;
  }
  else if(answer == 2)
  {
    Sim80x.Gsm.MsgFormat = GsmMsgFormat_Text;
    return GsmMsgFormat_Text;
  }
  else
    return GsmMsgFormat_Error;  
}
//######################################################################################################
bool  LIB_SIM800C::Gsm_setMsgFormat(GsmMsgFormat_t GsmMsgFormat)
{
  uint8_t   answer;
  if(GsmMsgFormat == GsmMsgFormat_PDU)
    answer = sim800c.sendAtCommand("AT+CMGF=0\r\n",1000,2,"AT+CMGF=0\r\r\nOK\r\n","AT+CMGF=0\r\r\nERROR\r\n");
  else  if(GsmMsgFormat == GsmMsgFormat_Text)
    answer = sim800c.sendAtCommand("AT+CMGF=1\r\n",1000,2,"AT+CMGF=1\r\r\nOK\r\n","AT+CMGF=1\r\r\nERROR\r\n");
  else
    return false;
  if(answer == 1)
  {
    if(GsmMsgFormat == GsmMsgFormat_PDU)
      Sim80x.Gsm.MsgFormat = GsmMsgFormat_PDU;
    if(GsmMsgFormat == GsmMsgFormat_Text)
      Sim80x.Gsm.MsgFormat = GsmMsgFormat_Text;
    return true;    
  }
  else
    return false;  
}
//######################################################################################################
GsmMsgMemory_t  LIB_SIM800C::getSmsMsgMemorySts(void)
{
  uint8_t   answer;
  answer = sim800c.sendAtCommand("AT+CPMS?\r\n",1000,2,"AT+CPMS?\r\r\n+CPMS: \"SM\"","AT+CPMS?\r\r\n+CPMS: \"ME\"");
  if(answer == 1)
    Sim80x.Gsm.MsgMemory = GsmMsgMemory_OnSim;
  else if(answer == 2)
    Sim80x.Gsm.MsgMemory = GsmMsgMemory_OnModule;
  else
    Sim80x.Gsm.MsgMemory = GsmMsgMemory_Error;
  return  Sim80x.Gsm.MsgMemory; 
}
//######################################################################################################
bool  LIB_SIM800C::Gsm_setMsgMemoryLocation(GsmMsgMemory_t GsmMsgMemory)
{
  uint8_t   answer;
  if(GsmMsgMemory == GsmMsgMemory_OnSim)
  {
    answer = sim800c.sendAtCommand("AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n",1000,1,"\r\n+CPMS:");
    if(answer == 1)
    {
      Sim80x.Gsm.MsgMemory = GsmMsgMemory_OnSim;
      return true;
    }
    else
      return false;
  }
  if(GsmMsgMemory == GsmMsgMemory_OnModule)
  {
    answer = sim800c.sendAtCommand("AT+CPMS=\"ME\",\"ME\",\"ME\"\r\n",1000,1,"\r\n+CPMS:");
    if(answer == 1)
    {
      Sim80x.Gsm.MsgMemory = GsmMsgMemory_OnModule;
      return true;
    }
    else
      return false;
  }
  return false;
}
//######################################################################################################
GsmTECharacterSet_t     LIB_SIM800C::Gsm_getMsgCharacterFormat(void)
{
  Sim80x.Gsm.TeCharacterFormat = (GsmTECharacterSet_t)sim800c.sendAtCommand("AT+CSCS?\r\n",1000,7,"\r\n+CSCS: \"GSM\"\r\n","\r\n+CSCS: \"UCS2\"\r\n","\r\n+CSCS: \"IRA\"\r\n","\r\n+CSCS: \"HEX\"\r\n","\r\n+CSCS: \"PCCP\"\r\n","\r\n+CSCS: \"PCDN\"\r\n","\r\n+CSCS: \"8859-1\"\r\n");
  return Sim80x.Gsm.TeCharacterFormat;
}
//######################################################################################################
bool  LIB_SIM800C::Gsm_setMsgCharacterFormat(GsmTECharacterSet_t GsmTECharacterSet)
{
  uint8_t answer;
  switch(GsmTECharacterSet)
  {
    case GsmTECharacterSet_Error:
    return false;
    case GsmTECharacterSet_GSM:
      answer = sim800c.sendAtCommand("AT+CSCS=\"GSM\"\r\n",1000,2,"AT+CSCS=\"GSM\"\r\r\nOK\r\n","AT+CSCS=\"GSM\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_UCS2:
      answer = sim800c.sendAtCommand("AT+CSCS=\"UCS2\"\r\n",1000,2,"AT+CSCS=\"UCS2\"\r\r\nOK\r\n","AT+CSCS=\"UCS2\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_IRA:
      answer = sim800c.sendAtCommand("AT+CSCS=\"IRA\"\r\n",1000,2,"AT+CSCS=\"IRA\"\r\r\nOK\r\n","AT+CSCS=\"IRA\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_HEX:
      answer = sim800c.sendAtCommand("AT+CSCS=\"HEX\"\r\n",1000,2,"AT+CSCS=\"HEX\"\r\r\nOK\r\n","AT+CSCS=\"HEX\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_PCCP:
      answer = sim800c.sendAtCommand("AT+CSCS=\"PCCP\"\r\n",1000,2,"AT+CSCS=\"PCCP\"\r\r\nOK\r\n","AT+CSCS=\"PCCP\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_PCDN:
      answer = sim800c.sendAtCommand("AT+CSCS=\"PCDN\"\r\n",1000,2,"AT+CSCS=\"PCDN\"\r\r\nOK\r\n","AT+CSCS=\"PCDN\"\r\r\n+CME ERROR");  
    break;
    case GsmTECharacterSet_8859_1:
      answer = sim800c.sendAtCommand("AT+CSCS=\"8859-1\"\r\n",1000,2,"AT+CSCS=\"8859-1\"\r\r\nOK\r\n","AT+CSCS=\"8859-1\"\r\r\n+CME ERROR");  
    break;   
  }  
  Gsm_getMsgCharacterFormat();
  if(answer == 1)
    return true;
  else
    return false;
}
//######################################################################################################
bool  LIB_SIM800C::getSmsMsg(uint8_t index)
{
  uint8_t answer;
  char str[16];
  memset(Sim80x.Gsm.Msg,0,sizeof(Sim80x.Gsm.Msg));
  memset(Sim80x.Gsm.MsgDate,0,sizeof(Sim80x.Gsm.MsgDate));
  memset(Sim80x.Gsm.MsgNumber,0,sizeof(Sim80x.Gsm.MsgNumber));
  memset(Sim80x.Gsm.MsgTime,0,sizeof(Sim80x.Gsm.MsgTime));  
  sprintf(str,"AT+CMGR=%d\r\n",index);
  answer = sim800c.sendAtCommand(str,5000,2,"\r\nOK\r\n","\r\nERROR\r\n");
  if((answer == 1 ) && (Sim80x.Gsm.MsgReadIsOK==1))
    return true;
  else
    return false;    
}
//######################################################################################################
bool  LIB_SIM800C::deleteSmsMsg(uint8_t index)
{
  uint8_t answer;
  char str[16];
  char strP1[22];
  char strP2[22];
  sprintf(str,"AT+CMGD=%d\r\n",index);
  sprintf(strP1,"AT+CMGD=%d\r\r\nOK\r\n",index);
  sprintf(strP2,"AT+CMGD=%d\r\r\nERROR\r\n",index);
  answer = sim800c.sendAtCommand(str,25000,2,strP1,strP2);
  if(answer == 1)
    return true;
  else
    return false;  
}
//######################################################################################################
bool  LIB_SIM800C::Gsm_getMsgServiceNumber(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CSCA?\r\n",5000,1,"\r\n+CSCA:");
  if((answer == 1) && (Sim80x.Gsm.MsgServiceNumber[0]!=0))
    return true;
  else
    return false;
}
//######################################################################################################
bool  LIB_SIM800C::setSmsMsgServiceNo(char *ServiceNumber)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+CSCA=\"%s\",145\r\n",ServiceNumber);
  snprintf(strParam,sizeof(str),"AT+CSCA=\"%s\"\r\r\nOK\r\n",ServiceNumber);
  answer = sim800c.sendAtCommand(str,5000,1,strParam);
  if(answer == 1)
    return true;
  else
    return false;
}
//######################################################################################################
bool  LIB_SIM800C::getSmsMsgTextModeParam(void)
{
  uint8_t answer;
  answer = sim800c.sendAtCommand("AT+CSMP?\r\n",500,1,"\r\nOK\r\n");
  if(answer == 1)
    return true;
  else
    return false;  
}
//######################################################################################################
bool  LIB_SIM800C::Gsm_setMsgTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs)
{
  uint8_t answer;
  char str[32];
  char strParam[32];
  snprintf(str,sizeof(str),"AT+CSMP=%d,%d,%d,%d\r\n",fo,vp,pid,dcs);
  snprintf(strParam,sizeof(strParam),"AT+CSMP=%d,%d,%d,%d\r\nOK\r\n",fo,vp,pid,dcs);
  answer = sim800c.sendAtCommand(str,500,1,strParam);
  if(answer == 1)
    return true;
  else
    return false;
}
//######################################################################################################
bool  LIB_SIM800C::setSmsMsgTxt(char *Number,char *msg)
{
  uint8_t answer;
  uint8_t Timeout=60;
  char str[128];
  Sim80x.Gsm.MsgSent=0;
  if(Sim80x.Gsm.MsgFormat != GsmMsgFormat_Text)
    Gsm_setMsgFormat(GsmMsgFormat_Text);
  snprintf(str,sizeof(str),"AT+CMGS=\"%s\"\r\n",Number);
  answer = sim800c.sendAtCommand(str,10000,1,"\r\r\n> ");
  delay_ms(100);
  if(answer != 1)
  {
    snprintf(str,sizeof(str),"%c",27);
    sim800c.writeString(str);
    return false;
  }
  strcpy(Sim80x.Gsm.MsgSentNumber,Number);
  snprintf(str,sizeof(str),"%s%c",msg,26);
  sim800c.writeString(str);
  while(Timeout>0)
  {
    delay_ms(1000);
    Timeout--;
    if(Sim80x.Gsm.MsgSent == 1)
      return true;
  }
  return false;      
}
//######################################################################################################

//######################################################################################################

#endif
#endif
