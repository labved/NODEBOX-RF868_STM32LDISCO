#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_HC05.h"
#include "LIB_HC05.h"

#include "HC05_SHORTCODES.h"
#include "HC05_TYPEDEF.h"

#ifdef LIB_HC05_H

#if (_USE_BT == 1)

HAL_HC05 hc05;
Sim80x_t sim80x; 

/****************************************************************
*FUNCTION NAME:test
*FUNCTION     :test codes
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::test(void)
{
  char      *strStart, *str1;
  setAtMode();
  
  USART_ITConfig(HC05_USART_CH, USART_IT_RXNE, ENABLE);
  hc05.sendAtCommand("AT+VERSION?\r\n", 200, 0);

  // Read & Update Operation
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+VERSION:");
  
  if(str1!=NULL)
    sscanf(str1,"+VERSION:%s",Sim80x.IMEI);    

  debugTerminal("HC05_HC05_getVersion");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

                     
}

/****************************************************************
*FUNCTION NAME:powerOnReset
*FUNCTION     :Switch reset 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::powerOnReset(void)
{
    hc05.hardReset(); 
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :HC05 register config 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::regConfigSettings(void)
{
  setAtMode();
  setFactoryDefault(200);
}

/****************************************************************
*FUNCTION NAME:setATMode
*FUNCTION     :set device to respond to AT Command Mode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::setAtMode(void)
 { 
   GPIO_ResetBits(HC05_GPIO_EN, HC05_PIN_EN);      //  EN pulled to LOW
   delay_ms(100);
   
   GPIO_SetBits(HC05_GPIO_KEY, HC05_PIN_KEY);      //  KEY PIN HIGH TO ON COMMAND AT MODE
   delay_ms(300);
    
   GPIO_SetBits(HC05_GPIO_EN, HC05_PIN_EN);       // EN floating to HIGH
   delay_ms(1000);   
 }

 /****************************************************************
*FUNCTION NAME:setDataMode
*FUNCTION     :set device to respond to Data Mode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::setDataMode(void)
{
  GPIO_ResetBits(HC05_GPIO_EN, HC05_PIN_EN);      //  POWER OFF
  delay_ms(100);
  
  GPIO_ResetBits(HC05_GPIO_KEY, HC05_PIN_KEY);    //  KEY PIN LOW TO ON DATA MODE
  delay_ms(300);

  GPIO_SetBits(HC05_GPIO_EN, HC05_PIN_EN);         //POWER ON
  delay_ms(1000);   

}

/****************************************************************
*FUNCTION NAME:debugTerminal
*FUNCTION     :Prints response to terminal IO
*INPUT        :message
*OUTPUT       :void
****************************************************************/
void LIB_HC05::debugTerminal(byte const *msg)
{
  char      *strStart, *str1;

  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
  str1 = strstr(strStart,"OK\r\n");
  #if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
  
     strStart = (char*)&Sim80x.UsartRxBuffer[0];  
    str1 = strstr(strStart,"OK\r\n");

      if(sizeof(str1)!=0)
      {         
        #if (_SIM80X_DEBUG==2)
          printf("\r\nAT Respone : %s",Sim80x.UsartRxBuffer);
        #endif
        printf("\r\n%s ---> OK\r\n", msg);
        
      }
      else
      {          
        printf("\r\n% ---> FAILED\r\n", msg);
      }
    
  #endif
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :initialization
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_HC05::Init(void)
{
  hc05.gpioInit();
  hc05.uartInit(HC05_BAUDRATE_AT);
  powerOnReset();

  regConfigSettings();
}

/****************************************************************
*FUNCTION NAME:softReset
*FUNCTION     :soft Reset
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::softReset(uint16_t timeout)
{
  uint8_t answer;

  answer = (hc05.sendAtCommand("AT+RESET", timeout, 0)); // CHECK
  
  debugTerminal("HC05_softReset");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:probe
*FUNCTION     :probe
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::probe(uint16_t timeout)
{
  startOperation(timeout);
    
  hc05.sendAtCommand("AT\r\n",200,0);
  
  debugTerminal("HC05_probe");

  return hc05.readOperationResult();
}


/****************************************************************
*FUNCTION NAME:getVersion
*FUNCTION     :get version number 
*INPUT        :buffer, buffer_size, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getVersion(byte *buffer, size_t buffer_size, uint16_t timeout)
{
  uint8_t answer;

  startOperation(timeout);

  if (!buffer || buffer_size <= 1)
    return false;
  answer = hc05.sendAtCommand("AT+VERSION?\r\n", 200, 0);
  
  debugTerminal("HC05_getVersion");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;
  
  /* Response should look like "+VERSION:2.0-20100601" */

} 

/****************************************************************
*FUNCTION NAME:getAddress
*FUNCTION     :getAddress
*INPUT        :address, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getAddress(BluetoothAddress &address, uint16_t timeout)
{
  return hc05.readAddressWithCommand(address, AT_ADDR, timeout);

}

/****************************************************************
*FUNCTION NAME:getName
*FUNCTION     :get Name from external device
*INPUT        :buffer, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getName(char *buffer, uint16_t timeout)
{
  uint8_t answer;
  
  startOperation(timeout);

  if (!buffer)
  {
    m_errCode = HC05_FAIL;
    return false;
  }
  
  answer = hc05.sendAtCommand("AT+NAME?\r\n",200,0);

  debugTerminal("HC05_getName");    
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));
  
  if(answer == 1)
      return true;
  else
      return false;
  
}

/****************************************************************
*FUNCTION NAME:getRemoteDeviceName
*FUNCTION     :get Remote Device Name 
*INPUT        :address, buffer, buffer_size, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getRemoteDeviceName(const BluetoothAddress &address,
  char *buffer, size_t buffer_size, uint16_t timeout)
{  
  uint8_t answer;

  startOperation(timeout);

  if (!buffer || !buffer_size)
  {
    m_errCode = HC05_FAIL;
    return false;
  }

  char address_str[HC05_ADDRESS_BUFSIZE];
  hc05.printBluetoothAddress(address_str, address, ',');

  answer = hc05.sendAtCommand("AT+RNAME?\r\n", timeout, 0); //check

  debugTerminal("HC05_getRemoteDeviceName");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:getRole
*FUNCTION     :getRole
*INPUT        :HC05 role, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getRole(HC05_Role &role, uint16_t timeout)
{
  uint8_t answer;

  startOperation(timeout);
  
  answer =hc05.sendAtCommand("AT+ROLE?\r\n", timeout, 0);

  debugTerminal("HC05_setSimAccess");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:getDeviceClass
*FUNCTION     :get Device Class 
*INPUT        :device_class, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getDeviceClass(uint32_t &device_class, uint16_t timeout)
{
  uint8_t answer;

  startOperation(timeout);
  
  answer = hc05.sendAtCommand("AT+CLASS?\r\n", timeout, 0 );
  
  debugTerminal("HC05_getDeviceClass");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;


}

/****************************************************************
*FUNCTION NAME:getInquiryAccessCode
*FUNCTION     :getInquiryAccessCode
*INPUT        :iac, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryAccessCode(uint32_t &iac, uint16_t timeout)
{
  startOperation(timeout);

  uint8_t answer; 
  
  answer = hc05.sendAtCommand("AT+IAC?\r\n", timeout, 0);
  
  debugTerminal("HC05_getInquiryAccessCode");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;


}

/****************************************************************
*FUNCTION NAME:getInquiryMode
*FUNCTION     :getInquiryMode
*INPUT        :inq_mode, max_devices, max_duration, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryMode(HC05_InquiryMode &inq_mode,
  int16_t &max_devices, uint8_t &max_duration, uint16_t timeout)
{
  startOperation(timeout);

  uint8_t answer; 

  answer = hc05.sendAtCommand("AT+INQM?\r\n", timeout, 0);

  debugTerminal("HC05_getInquiryMode");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:getPassword
*FUNCTION     :getPassword
*INPUT        :buffer, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getPassword(char *buffer, uint16_t timeout)
{  
  uint8_t answer; 

  startOperation(timeout);  

  if (!buffer)
  {
    m_errCode = HC05_FAIL;
    return false;
  }

  answer = hc05.sendAtCommand("AT+PSWD?\r\n", timeout, 0);

  debugTerminal("HC05_getPassword");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:getSerialMode
*FUNCTION     :getSerialMode
*INPUT        :speed, stop_bits,  parity, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSerialMode(uint32_t &speed, uint8_t &stop_bits,
  HC05_Parity &parity, uint16_t timeout)
{
  uint8_t answer; 
  
  startOperation(timeout);
  
  answer = hc05.sendAtCommand("AT+UART?\r\n", timeout, 0);
  
  debugTerminal("HC05_getSerialMode");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  
  if(answer == 1)
      return true;
  else
      return false;
  
}

/****************************************************************
*FUNCTION NAME:getConnectionMode
*FUNCTION     :getConnectionMode  
*INPUT        :connection_mode, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getConnectionMode(
  HC05_Connection &connection_mode, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+CMODE?\r\n", timeout, 0);

  debugTerminal("HC05_getConnectionMode");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
 
  if(answer == 1)
      return true;
  else
      return false;
      
}

/****************************************************************
*FUNCTION NAME:getAddressBound
*FUNCTION     :getAddressBound
*INPUT        :BluetoothAddress, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getAddressBound(BluetoothAddress &address, uint16_t timeout)
{
  return hc05.readAddressWithCommand(address, AT_BIND, timeout);
}

/****************************************************************
*FUNCTION NAME:getLeds
*FUNCTION     :get Leds status
*INPUT        :led_status, led_connection, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getLeds(bool &led_status,
  bool &led_connection, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+POLAR?\r\n" ,timeout ,0 );

  debugTerminal("HC05_getLeds");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

    if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:getMultiplePorts
*FUNCTION     :getMultiplePorts
*INPUT        :port_states, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getMultiplePorts(uint16_t &port_states, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+MPIO?\r\n", timeout, 0 );

  debugTerminal("HC05_getMultiplePorts");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:getInquiryAndPagingParams
*FUNCTION     :getInquiryAndPagingParams
*INPUT        :inquiry_interval, inquiry_duration, paging_interval, paging_duration, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryAndPagingParams(uint16_t &inquiry_interval, uint16_t &inquiry_duration, 
uint16_t &paging_interval, uint16_t &paging_duration, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+IPSCAN?\r\n", timeout, 0);

  debugTerminal("HC05_getInquiryAndPagingParams");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
  
}

/****************************************************************
*FUNCTION NAME:getSniffParams
*FUNCTION     :getSniffParams
*INPUT        :max_time, min_time, retry_interval, sniff_timeout, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSniffParams(uint16_t &max_time, uint16_t &min_time,
  uint16_t &retry_interval, uint16_t &sniff_timeout, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+SNIFF?\r\n", timeout, 0 );

  debugTerminal("HC05_setInquiryAndPagingParams");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
  
}

/****************************************************************
*FUNCTION NAME:getSecurityAndEncryption
*FUNCTION     :getSecurityAndEncryption 
*INPUT        :Security,encryption, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSecurityAndEncryption(HC05_Security &security,
  HC05_Encryption &encryption, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+SENM?\r\n", timeout, 0);

  debugTerminal("HC05_getSecurityAndEncryption");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

  
}

/****************************************************************
*FUNCTION NAME:getState
*FUNCTION     :getState
*INPUT        :state, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getState(HC05_State &state, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+STATE?\r\n", timeout, 0);

  debugTerminal("HC05_getState");   

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
      
}

/****************************************************************
*FUNCTION NAME:getLastAuthenticatedDevice
*FUNCTION     :getLastAuthenticatedDevice  
*INPUT        :address, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getLastAuthenticatedDevice(
  BluetoothAddress &address, uint16_t timeout)
{
  return hc05.readAddressWithCommand(address, AT_MRAD, timeout);
}

/****************************************************************
*FUNCTION NAME:setFactoryDefault
*FUNCTION     :set to factory settings
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setFactoryDefault(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand("AT+ORGL\r\n", 200, 0);  

  debugTerminal("HC05_setFactoryDefault");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:setName
*FUNCTION     :set Name to extenal device
*INPUT        :name, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setName(const char *name, uint16_t timeout)
{
  char temp[20];    
  uint8_t answer;
    
  snprintf(temp, sizeof(temp), "AT+NAME=%s\r\n", name); 
  answer =hc05.sendAtCommand(temp,200, 0);

  debugTerminal("HC05_setName");
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:setRole
*FUNCTION     :setRole
*INPUT        :role, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setRole(HC05_Role role, uint16_t timeout)
{
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+ROLE=%d", role);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setRole");
 
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
 
  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setDeviceClass
*FUNCTION     :set Device Class
*INPUT        :device_class, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setDeviceClass(uint32_t device_class, uint16_t timeout)
{
  char temp[10];
  uint8_t answer;

  snprintf(temp, sizeof(temp), "AT+CLASS=%lx", device_class);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setDeviceClass");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

  }

/****************************************************************
*FUNCTION NAME:setInquiryAccessCode
*FUNCTION     :setInquiryAccessCode
*INPUT        :iac, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryAccessCode(uint32_t iac, uint16_t timeout)
{
  char iac_str[20];
  uint8_t answer;

  snprintf(iac_str, sizeof(iac_str), "AT+IAC=%lx", iac);
  answer =hc05.sendAtCommand(iac_str, timeout, 0);

  debugTerminal("HC05_getInquiryAccessCode");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:setInquiryMode
*FUNCTION     :setInquiryMode
*INPUT        :inq_mode, max_devices, max_duration, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryMode(HC05_InquiryMode inq_mode,
  int16_t max_devices, uint8_t max_duration, uint16_t timeout)
{
  char mode[20];
  uint8_t answer;
  
  /* Yeah, max_devices is signed 16-bit integer, but the module accepts
   * an unsigned 16-bit integer while actually interpreting it as signed.
   *                          <-^->
   * Tricky chinese engineers (-_-)
   *                            "
   */
 
  snprintf(mode, sizeof(mode), "AT+INQM=%d,%u,%u", inq_mode, (uint16_t)max_devices, max_duration);
  answer =hc05.sendAtCommand(mode, timeout, 0);

  debugTerminal("HC05_setInquiryMode");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:setPassword
*FUNCTION     :setPassword
*INPUT        :password, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setPassword(const char *password, uint16_t timeout)
{
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+PSWD=%s\r\n", password); 
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setPassword");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
  }

/****************************************************************
*FUNCTION NAME:setSerialMode
*FUNCTION     :setSerialMode
*INPUT        :speed, stop_bits,  parity, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSerialMode(uint32_t speed,
  uint8_t stop_bits, HC05_Parity parity, uint16_t timeout)
{
  stop_bits -= 1; // 0: 1 stop bit, 1: 2 stop bits, any other are not allowed
  char temp[20];
  uint8_t answer;

  snprintf(temp, sizeof(temp), "AT+UART=%lu,%u,%u", speed, stop_bits, parity);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setSerialMode");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:setConnectionMode
*FUNCTION     :setConnectionMode
*INPUT        :connection_mode, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setConnectionMode(
  HC05_Connection connection_mode, uint16_t timeout)
{
  char temp[20];
  uint8_t answer;

  snprintf(temp, sizeof(temp), "AT+CMODE=%u", connection_mode);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setConnectionMode");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setLeds
*FUNCTION     :set Leds status
*INPUT        :led_status, led_connection, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setLeds(bool led_status,
  bool led_connection, uint16_t timeout)
{
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+%d,%d",
    (led_status ? 1 : 0), (led_connection ? 1 : 0));
  answer = hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setLeds");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setPortState
*FUNCTION     :set HC05 Ports States
*INPUT        :port_states, port_num, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setPortState(uint8_t port_num,
  uint8_t port_state, uint16_t timeout)
{
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+PIO=%u,%u", port_num, port_state);
  answer = hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setPortState");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setMultiplePorts
*FUNCTION     :setMultiplePorts  
*INPUT        :port_states, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setMultiplePorts(uint16_t port_states, uint16_t timeout)
{
  char temp[20];
  uint8_t answer;

  snprintf(temp, sizeof(temp), "AT+MPIO=%x", port_states);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setMultiplePorts");  
  
  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
  if(answer == 1)
      return true;
  else
      return false;
  
}

/****************************************************************
*FUNCTION NAME:setInquiryAndPagingParams
*FUNCTION     :setInquiryAndPagingParams
*INPUT        :inquiry_interval, inquiry_duration, paging_interval, paging_duration, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryAndPagingParams(
  uint16_t inquiry_interval, uint16_t inquiry_duration,
  uint16_t paging_interval, uint16_t paging_duration, uint16_t timeout)
{ 
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+IPSCAN=%u,%u,%u,%u",
    inquiry_interval, inquiry_duration, paging_interval, paging_duration);
  answer = hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setInquiryAndPagingParams");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setSniffParams
*FUNCTION     :set Sniff Parameter
*INPUT        :max_time, min_time, retry_interval, sniff_timeout, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, uint16_t timeout)
{
  char temp[20];
  uint8_t answer;

  snprintf(temp, sizeof(temp), "AT+SNIFF=%u,%u,%u,%u",
    max_time, min_time, retry_interval, sniff_timeout);
  answer =hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setInquiryAndPagingParams");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:setSecurityAndEncryption
*FUNCTION     :setSecurityAndEncryption
*INPUT        :security, encryption, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSecurityAndEncryption(HC05_Security security,
  HC05_Encryption encryption, uint16_t timeout)
{
  uint8_t answer;
  char temp[20];

  snprintf(temp, sizeof(temp), "AT+SENM=%u,%u", security, encryption);
  answer = hc05.sendAtCommand(temp, timeout, 0);

  debugTerminal("HC05_setSecurityAndEncryption");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}


/****************************************************************
*FUNCTION NAME:bind
*FUNCTION     :bind
*INPUT        :address, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::bind(const BluetoothAddress &address, uint16_t timeout)
{
  return hc05.writeAddressWithCommand(address, AT_BIND, timeout);
}

/****************************************************************
*FUNCTION NAME:enterSniffMode
*FUNCTION     :enterSniffMode
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::enterSniffMode(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand("AT+ENSNIFF", timeout, 0);

  debugTerminal("HC05_enterSniffMode");  

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:exitSniffMode
*FUNCTION     :exitSniffMode
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::exitSniffMode(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand("AT+EXSNIFF", timeout, 0);

  debugTerminal("HC05_exitSniffMode");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:deleteDeviceFromList
*FUNCTION     :deleteDeviceFromList
*INPUT        :address : BluetoothAddress, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::deleteDeviceFromList(
    const BluetoothAddress &address, uint16_t timeout)
{
  return hc05.writeAddressWithCommand(address, AT_RMSAD, timeout);
}

/****************************************************************
*FUNCTION NAME:deleteAllDevicesFromList
*FUNCTION     :deleteAllDevicesFromList
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::deleteAllDevicesFromList(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand("AT+RMAAD", timeout, 0);

  debugTerminal("HC05_deleteAllDevicesFromList");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:findDeviceInList
*FUNCTION     :findDeviceInList 
*INPUT        :address : Bluetoothaddress, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::findDeviceInList(
  const BluetoothAddress &address, uint16_t timeout)
{
  return hc05.writeAddressWithCommand(address, AT_FSAD, timeout);
}

/****************************************************************
*FUNCTION NAME:countDevicesInList
*FUNCTION     :countDevicesInList  
*INPUT        :device_count, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::countDevicesInList(uint8_t &device_count, uint16_t timeout)
{
  uint8_t answer; 

  startOperation(timeout);

  answer = hc05.sendAtCommand("AT+ADCN?\r\n", timeout, 0);

  debugTerminal("HC05_countDevicesInList");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

  if(answer == 1)
      return true;
  else
      return false;

}

/****************************************************************
*FUNCTION NAME:initSerialPortProfile
*FUNCTION     :initSerialPortProfile  
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::initSerialPortProfile(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand("AT+INIT", timeout, 0);

  debugTerminal("HC05_initSerialPortProfile");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:inquire
*FUNCTION     :inquire
*INPUT        :callback, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::inquire(InquiryCallback callback, uint16_t timeout)
{
  uint8_t answer; 
  BluetoothAddress address;
  const char *address_part;

  startOperation(timeout);
    
  answer = hc05.sendAtCommand("AT+INQ\r\n", timeout, 0);

  debugTerminal("HC05_inquire");

  while (!isOperationTimedOut())
  {
    //if (m_uart->peek() != '+')
    if(USART_ReceiveData(HC05_USART_CH) != '+')
      break;

    char response[HC05_ADDRESS_BUFSIZE + 10];

    address_part = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_INQ);


    hc05.parseBluetoothAddress(address, address_part, ':');

    if (callback)
      callback(address);
  }

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:cancelInquiry
*FUNCTION     :cancelInquiry  
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::cancelInquiry(uint16_t timeout)
{
  uint8_t answer;

  answer =hc05.sendAtCommand(AT_INQC, 0, timeout);

  debugTerminal("HC05_cancelInquiry");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 

  if(answer == 1)
      return true;
  else
      return false;
}

/****************************************************************
*FUNCTION NAME:pair
*FUNCTION     :pairing AT commands  
*INPUT        :address : BluetoothAddress, timeout
*OUTPUT       :bool
****************************************************************/
//bool LIB_HC05::pair(const BluetoothAddress &address, uint16_t timeout)
//{
//  uint8_t answer;
//  char params_str[HC05_ADDRESS_BUFSIZE + 15];
//
//  int address_length = hc05.printBluetoothAddress(params_str, address, ',');
//
//  snprintf(params_str + address_length,
//    sizeof(params_str) - address_length, "AT+PAIR,%lu", timeout);
//  answer =hc05.sendAtCommand(params_str, timeout, 0);
//
//  debugTerminal("HC05_pair");  
//
//  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand)); 
//
//  if(answer == 1)
//      return true;
//  else
//      return false;
//  
//}

/****************************************************************
*FUNCTION NAME:connect
*FUNCTION     :connect  
*INPUT        :address : BluetoothAddress, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::connect(const BluetoothAddress &address, uint16_t timeout)
{
  return hc05.writeAddressWithCommand(address, "LINK", timeout);
}

/****************************************************************
*FUNCTION NAME:disconnect
*FUNCTION     :disconnect commands
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::disconnect(uint16_t timeout)
{
  uint8_t answer;

  startOperation(timeout);
    
  answer =hc05.sendAtCommand("AT+DISC\r\n", timeout, 0 );

  debugTerminal("HC05_pair");

  memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

  if(answer == 1)
      return true;
  else
      return false;


}

//new functions
/****************************************************************
*FUNCTION NAME:processversion
*FUNCTION     :processversion
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processVersion(uint8_t answer, char *buffer, size_t buffer_size, uint16_t addrs)
{
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"+VERSION:");

  if(str1!=NULL)
    sscanf(str1,"+VERSION:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getversion");

  if(str1!=NULL)
    return true;
  else
    return false;   
  	
}

/****************************************************************
*FUNCTION NAME:processRemoteDeviceName
*FUNCTION     :processRemoteDeviceName
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processRemoteDeviceName(const BluetoothAddress &address,
  char *buffer, size_t buffer_size, uint16_t addrs)
{
  char      *strStart, *str1;
  char      *temp;

  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+RNAME:");

  if(str1!=NULL)
    sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getRemoteDeviceName");

  if(str1!=NULL)
    return true;
  else
    return false;   

}

/****************************************************************
*FUNCTION NAME:processDeviceClass
*FUNCTION     :processDeviceClass
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processDeviceClass(uint32_t &device_class, uint16_t addrs)
{
   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+RNAME:");

  if(str1!=NULL)
    sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getRemoteDeviceName");

  if(str1!=NULL)
    return true;
  else
    return false; 
    
}

/****************************************************************
*FUNCTION NAME:process
*FUNCTION     :process
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processInquiryAccessCode( uint32_t &iac, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+IAC:");

  if(str1!=NULL)
    sscanf(str1,"+IAC:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getInquiryAccessCode");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:process
*FUNCTION     :process
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processInquiryMode(HC05_InquiryMode &inq_mode,
  int16_t &max_devices, uint8_t &max_duration, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+INQM:");

  if(str1!=NULL)
    sscanf(str1,"+INQM:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getInquiryMode");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processPassword
*FUNCTION     :processPassword
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processPassword(char *buffer, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+PSWD:");

  if(str1!=NULL)
    sscanf(str1,"+PSWD:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getPassword");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processSerialMode
*FUNCTION     :processSerialMode
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processSerialMode(uint32_t &speed, uint8_t &stop_bits,
  HC05_Parity &parity, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+UART:");

  if(str1!=NULL)
    sscanf(str1,"+UART:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_SerialMode");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processConnectionMode
*FUNCTION     :processConnectionMode
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processConnectionMode(
  HC05_Connection &connection_mode, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+CMOD:");

  if(str1!=NULL)
    sscanf(str1,"+CMOD:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_ConnectionMode");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processLed
*FUNCTION     :processLed
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processLed(bool &led_status,
  bool &led_connection, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+POLAR:");

  if(str1!=NULL)
    sscanf(str1,"+POLAR:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getLedStatus");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processPortState
*FUNCTION     :processPortState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
//bool LIB_HC05::processPortState(uint8_t port_num,
//  uint8_t port_state, uint16_t addrs)
//{   
//  char      *strStart, *str1;
//  char      *temp;
//
//
//  strStart = (char*)&sim80x.UsartRxBuffer[0];  
//
//  str1 = strstr(strStart,"+RNAME:");
//
//  if(str1!=NULL)
//    sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
//  
//  debugTerminal("HC05_PortState");
//
//  if(str1!=NULL)
//    return true;
//  else
//    return false; 
//
//}

/****************************************************************
*FUNCTION NAME:processMultiPortState
*FUNCTION     :processMultiPortState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processMultiPortState(uint16_t &port_states, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+RNAME:");

  if(str1!=NULL)
    sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processMultiPortState");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processGetInquiryAndPagingParams
*FUNCTION     :processGetInquiryAndPagingParams
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processInquiryAndPagingParams(uint16_t &inquiry_interval, 
uint16_t &inquiry_duration, uint16_t &paging_interval, uint16_t &paging_duration, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+IPSCAN:");

  if(str1!=NULL)
    sscanf(str1,"+IPSCAN:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processGetInquiryAndPagingParams");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processSecurityAndEncryption
*FUNCTION     :processSecurityAndEncryption
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processSecurityAndEncryption(HC05_Security &security,
  HC05_Encryption &encryption, uint16_t addrs)
{
  char      *strStart, *str1;
  char      *temp;

  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+SENM:");

  if(str1!=NULL)
    sscanf(str1,"+SENM:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processSecurityAndEncryption");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processState
*FUNCTION     :processState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processState(HC05_State &state, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+STATE:");

  if(str1!=NULL)
    sscanf(str1,"+STATE:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processState");

  if(str1!=NULL)
    return true;
  else
    return false; 

}


/****************************************************************
*FUNCTION NAME:processPair
*FUNCTION     :processPair
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
//bool LIB_HC05::processPair(uint16_t addrs)
//{   
//  char      *strStart, *str1;
//  char      *temp;
//
//
//  strStart = (char*)&sim80x.UsartRxBuffer[0];  
//
// // str1 = strstr(strStart,"+RNAME:");
//
//  if(str1!=NULL)
//  // sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
//  
//  debugTerminal("HC05_processPair");
//
//  if(str1!=NULL)
//    return true;
//  else
//    return false; 
//
//}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processDisconnect(uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+DISC:");

  if(str1!=NULL)
    sscanf(str1,"+DISC:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_HC05_processDisconnect");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processCountDevicesInList(uint8_t &device_count, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+ADCN:");

  if(str1!=NULL)
    sscanf(str1,"+ADCN:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processCountDevicesInList");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
/* bool LIB_HC05::processDeleteAllDevicesFromList(uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+RNAME:");

  if(str1!=NULL)
    sscanf(str1,"+RNAME:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getRemoteDeviceName");

  if(str1!=NULL)
    return true;
  else
    return false; 

}
	
	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"A/\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"A/\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getLastCommand");
  
} */

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+SNIFF:");

  if(str1!=NULL)
    sscanf(str1,"+SNIFF:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_getRemoteDeviceName");

  if(str1!=NULL)
    return true;
  else
    return false; 

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processRole(HC05_Role &role, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+ROLE:");

  if(str1!=NULL)
    sscanf(str1,"+ROLE:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processRole");

  if(str1!=NULL)
    return true;
  else
    return false; 

}
 

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processName(char *buffer, uint16_t addrs)
{   
  char      *strStart, *str1;
  char      *temp;


  strStart = (char*)&sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+NAME:");

  if(str1!=NULL)
    sscanf(str1,"+NAME:%s\r\nOK\r\n",temp);
  
  debugTerminal("HC05_processName");

  if(str1!=NULL)
    return true;
  else
    return false; 

}
 

#endif

#endif