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
  
  hc05.sendAtCommand("AT+VERSION?\r\n", 200, 0);
  
  // Read & Update Operation
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+VERSION:");
  
  if(str1!=NULL)
    sscanf(str1,"+VERSION:%s",Sim80x.IMEI);    

  debugTerminal("HC05_HC05_getVersion");

  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
                     
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
*FUNCTION NAME:return debugTerminal
*FUNCTION     :Prints response to terminal IO
*INPUT        :message
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::debugTerminal(char *msg)
{
  char      *strStart, *str1;

  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
  str1 = strstr(strStart,"OK\r\n");
  //#if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
   

     strStart = (char*)&Sim80x.UsartRxBuffer[0];  
    str1 = strstr(strStart,"OK\r\n");

      if(sizeof(str1)!=0)
      { 
        #if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
          #if (_SIM80X_DEBUG==2)
            printf("\r\nAT Respone : %s",Sim80x.UsartRxBuffer);
          #endif
          printf("\r\n%s ---> OK\r\n", msg);
        #endif
        return sizeof(str1);
      }
      else
      {
          
        #if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
          printf("\r\n% ---> FAILED\r\n", msg);
        #endif
        return 0;
      }
    
  //#endif
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
bool LIB_HC05::softReset(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(reset_cmd, "RESET");
  return (hc05.sendAtCommand("AT+RESET", timeout, 0)); // CHECK
}

/****************************************************************
*FUNCTION NAME:probe
*FUNCTION     :probe
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::probe(unsigned long timeout)
{
  startOperation(timeout);
  hc05.sendAtCommand("AT\r\n",200,0);
  
  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setFactoryDefault
*FUNCTION     :set to factory settings
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setFactoryDefault(unsigned long timeout)
{
  hc05.sendAtCommand("AT+ORGL\r\n", 200, 0);
  return debugTerminal("HC05_setFactoryDefault");
  
}

/****************************************************************
*FUNCTION NAME:getVersion
*FUNCTION     :get version number 
*INPUT        :buffer , buffer_size , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getVersion(char *buffer, size_t buffer_size, unsigned long timeout)
{
  uint8_t answer ;
  startOperation(timeout);

  if (!buffer || buffer_size <= 1)
    return false;

  //PGM_STRING_MAPPED_TO_RAM(command, "VERSION?");
  answer = hc05.sendAtCommand("AT+VERSION?\r\n", 200, 0);
   processVersion(buffer , buffer_size, 1);
  /* Response should look like "+VERSION:2.0-20100601" */
  char response[30];

  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+VERSION:");
  
 /*  const char *version = hc05.readResponseWithPrefix(
     response, sizeof(response), AT_RESP_VERSION);
  */
 /*   if (m_errCode != HC05_OK)
    return false;

  if (!version)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }
 
  snprintf(buffer, buffer_size, "%s", version);
  return answer ;*/
} 

/****************************************************************
*FUNCTION NAME:getAddress
*FUNCTION     :getAddress
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getAddress(BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "ADDR");
  return hc05.readAddressWithCommand(address, AT_ADDR, timeout);

}

/****************************************************************
*FUNCTION NAME:getName
*FUNCTION     :get Name from external device
*INPUT        :buffer , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getName(char *buffer, unsigned long timeout)
{
  startOperation(timeout);

  if (!buffer)
  {
    m_errCode = HC05_FAIL;
    return false;
  }

  //PGM_STRING_MAPPED_TO_RAM(command, "NAME?");
  hc05.sendAtCommand("AT+NAME?\r\n",200,0);
  return debugTerminal("HC05_getName");
  

  char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+NAME:");
  
  char *name_part = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_NAME);

  if (m_errCode != HC05_OK)
    return false;

  if (!name_part)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }

  //PGM_STRING_MAPPED_TO_RAM(format, "%s");
  
  snprintf(buffer, HC05_NAME_BUFSIZE, "%s", name_part);

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setName
*FUNCTION     :set Name to extenal device
*INPUT        :name, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setName(const char *name, unsigned long timeout)
{
 
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+NAME=%s\r\n", name); 
  hc05.sendAtCommand(temp,200, 0);
  return debugTerminal("HC05_setSimAccess");
  //PGM_STRING_MAPPED_TO_RAM(command, "NAME=");
 
  
}

/****************************************************************
*FUNCTION NAME:getRemoteDeviceName
*FUNCTION     :get Remote Device Name 
*INPUT        :address, buffer, buffer_size, timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getRemoteDeviceName(const BluetoothAddress &address,
  char *buffer, size_t buffer_size, unsigned long timeout)
{
  startOperation(timeout);

  if (!buffer || !buffer_size)
  {
    m_errCode = HC05_FAIL;
    return false;
  }

  char address_str[HC05_ADDRESS_BUFSIZE];
  hc05.printBluetoothAddress(address_str, address, ',');
  
  //PGM_STRING_MAPPED_TO_RAM(command, "RNAME?");
  
  hc05.sendAtCommand("AT+RNAME?\r\n", timeout, 0); //check
  debugTerminal("HC05_getRemoteDeviceName");
  processRemoteDeviceName(address, buffer, buffer_size, 1);
  char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+RNAME:");
  char *name_part = hc05.readResponseWithPrefix(response, sizeof(response), "+RNAME:");

  if (m_errCode != HC05_OK)
    return false;

  if (!name_part)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }

  //PGM_STRING_MAPPED_TO_RAM(format, "%s");
  snprintf(buffer, buffer_size, "%s", name_part);

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:getRole
*FUNCTION     :getRole
*INPUT        :HC05 role , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getRole(HC05_Role &role, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "ROLE?");
  hc05.sendAtCommand("AT+ROLE?\r\n", timeout, 0 );
  return debugTerminal("HC05_setSimAccess");
/* 
  char response[20];
 // PGM_STRING_MAPPED_TO_RAM(response_pattern, "+ROLE:");
  const char *role_str = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_ROLE);

  if (m_errCode != HC05_OK)
    return false;

  if (!role_str)
    return hc05.readOperationResult() && false;

  role = static_cast<HC05_Role>(atol(role_str));

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setRole
*FUNCTION     :setRole
*INPUT        :role , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setRole(HC05_Role role, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+ROLE=%d", role);

  //PGM_STRING_MAPPED_TO_RAM(command, "ROLE=");
  return hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setRole");

}

/****************************************************************
*FUNCTION NAME:getDeviceClass
*FUNCTION     :get Device Class 
*INPUT        :device_class , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getDeviceClass(uint32_t &device_class, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "CLASS?");
  hc05.sendAtCommand("AT+CLASS?\r\n", timeout, 0 );
  return debugTerminal("HC05_getDeviceClass");
   processDeviceClass(device_class, 1);

/* 
  device_class = 0;

  char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+CLASS:");
  const char *class_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_CLASS);

  if (m_errCode != HC05_OK)
    return false;

  if (!class_part)
    return hc05.readOperationResult() && false;

  device_class = htoul(class_part);

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setDeviceClass
*FUNCTION     :set Device Class
*INPUT        :device_class , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setDeviceClass(uint32_t device_class, unsigned long timeout)
{
  char temp[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%lx");
  snprintf(temp, sizeof(temp), "AT+CLASS=%lx", device_class);

 // PGM_STRING_MAPPED_TO_RAM(command, "CLASS=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setDeviceClass");

  }

/****************************************************************
*FUNCTION NAME:getInquiryAccessCode
*FUNCTION     :getInquiryAccessCode
*INPUT        :iac , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryAccessCode(uint32_t &iac, unsigned long timeout)
{
  startOperation(timeout);

 // PGM_STRING_MAPPED_TO_RAM(command, "IAC?");
  hc05.sendAtCommand("AT+IAC?\r\n", timeout, 0);
  debugTerminal("HC05_getInquiryAccessCode");
  processInquiryAccessCode(iac, 1);


  iac = 0;

  if (isOperationTimedOut())
    return false;

  char response[30];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+IAC:");
  const char *iac_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_IAC);

  if (m_errCode != HC05_OK)
    return false;

  if (!iac_part)
    return hc05.readOperationResult() && false;

  iac = htoul(iac_part);

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setInquiryAccessCode
*FUNCTION     :setInquiryAccessCode
*INPUT        :iac , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryAccessCode(uint32_t iac, unsigned long timeout)
{
  char iac_str[20];
  //PGM_STRING_MAPPED_TO_RAM(format, "%lx");
  snprintf(iac_str, sizeof(iac_str), "AT+IAC=%lx", iac);

  //PGM_STRING_MAPPED_TO_RAM(command, "IAC=");
  hc05.sendAtCommand(iac_str, timeout, 0);
  return debugTerminal("HC05_getInquiryAccessCode");
}

/****************************************************************
*FUNCTION NAME:getInquiryMode
*FUNCTION     :getInquiryMode
*INPUT        :inq_mode , max_devices , max_duration , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryMode(HC05_InquiryMode &inq_mode,
  int16_t &max_devices, uint8_t &max_duration, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "INQM?");
  hc05.sendAtCommand("AT+INQM?\r\n", timeout, 0);
  return debugTerminal("HC05_getInquiryMode");
  processInquiryMode(inq_mode, max_devices, max_duration, 1);
/* 
  inq_mode = HC05_INQUIRY_STANDARD;
  max_devices = 0;
  max_duration = 0;

  char response[30];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+INQM:");
  char *mode_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_INQM);

  if (m_errCode != HC05_OK)
    return false;

  if (!mode_part)
    return hc05.readOperationResult() && false;

  inq_mode = static_cast<HC05_InquiryMode>(atol(mode_part));
  mode_part = strchrnul(mode_part, ',');

  if (*mode_part != ',')
    return hc05.readOperationResult() && false;

  max_devices = atol(++mode_part);
  mode_part = strchrnul(mode_part, ',');

  if (*mode_part != ',')
    return hc05.readOperationResult() && false;

  max_duration = atol(++mode_part);

  return hc05.readOperationResult();
 */
}

/****************************************************************
*FUNCTION NAME:setInquiryMode
*FUNCTION     :setInquiryMode
*INPUT        :inq_mode , max_devices , max_duration , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryMode(HC05_InquiryMode inq_mode,
  int16_t max_devices, uint8_t max_duration, unsigned long timeout)
{
  char mode[20];
  
  /* Yeah, max_devices is signed 16-bit integer, but the module accepts
   * an unsigned 16-bit integer while actually interpreting it as signed.
   *                          <-^->
   * Tricky chinese engineers (-_-)
   *                            "
   */
  
  //PGM_STRING_MAPPED_TO_RAM(format, "%d,%u,%u");
  snprintf(mode, sizeof(mode), "AT+INQM=%d,%u,%u", inq_mode, (uint16_t)max_devices, max_duration);

  //PGM_STRING_MAPPED_TO_RAM(command, "INQM=");
  hc05.sendAtCommand(mode, timeout, 0);
  return debugTerminal("HC05_setInquiryMode");
}

/****************************************************************
*FUNCTION NAME:getPassword
*FUNCTION     :getPassword
*INPUT        :buffer , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getPassword(char *buffer, unsigned long timeout)
{
  startOperation(timeout);

  if (!buffer)
  {
    m_errCode = HC05_FAIL;
    return false;
  }

  //PGM_STRING_MAPPED_TO_RAM(command, "PSWD?");
  hc05.sendAtCommand("AT+PSWD?\r\n", timeout, 0);
  return debugTerminal("HC05_getPassword");
  processPassword(buffer, 1);

  char response[HC05_PASSWORD_MAXLEN + 15];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+PSWD:");
  const char *password_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_PSWD);

  if (m_errCode != HC05_OK)
  {
    *buffer = 0;
    return false;
  }

  if (!password_part)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }

  //PGM_STRING_MAPPED_TO_RAM(format, "%s");
  snprintf(buffer, HC05_PASSWORD_BUFSIZE, "%s", password_part);

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setPassword
*FUNCTION     :setPassword
*INPUT        :password , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setPassword(const char *password, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+PSWD=%s\r\n", password); 
  return  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setPassword");
  //PGM_STRING_MAPPED_TO_RAM(command, "PSWD=");
  }

/****************************************************************
*FUNCTION NAME:getSerialMode
*FUNCTION     :getSerialMode
*INPUT        :speed , stop_bits ,  parity , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSerialMode(uint32_t &speed, uint8_t &stop_bits,
  HC05_Parity &parity, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "UART?");
  hc05.sendAtCommand("AT+UART?\r\n", timeout, 0);
  return debugTerminal("HC05_getSerialMode");
  processSerialMode(speed, stop_bits,  parity, 1);
  

  char response[30];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+UART:");
  char *mode_str = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_UART);

  if (m_errCode != HC05_OK)
    return false;

  if (!mode_str)
    return hc05.readOperationResult() && false;

  speed = atol(mode_str);
  mode_str = strchrnul(mode_str, ',');

  if (*mode_str != ',')
  {
    m_errCode = HC05_FAIL;
    return hc05.readOperationResult() && false;
  }

  stop_bits = atol(++mode_str) + 1;
  mode_str = strchrnul(mode_str, ',');

  if (*mode_str != ',')
  {
    m_errCode = HC05_FAIL;
    return hc05.readOperationResult() && false;
  }

  parity = static_cast<HC05_Parity>(atol(++mode_str));

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setSerialMode
*FUNCTION     :setSerialMode
*INPUT        :speed , stop_bits ,  parity , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSerialMode(uint32_t speed,
  uint8_t stop_bits, HC05_Parity parity, unsigned long timeout)
{
  stop_bits -= 1; // 0: 1 stop bit, 1: 2 stop bits, any other are not allowed

  char temp[20];
  snprintf(temp, sizeof(temp), "AT+UART=%lu,%u,%u", speed, stop_bits, parity);

  //PGM_STRING_MAPPED_TO_RAM(command, "UART=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setSerialMode");
}

/****************************************************************
*FUNCTION NAME:getConnectionMode
*FUNCTION     :getConnectionMode  
*INPUT        :connection_mode , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getConnectionMode(
  HC05_Connection &connection_mode, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "CMODE?");
  hc05.sendAtCommand("AT+CMODE?\r\n", timeout, 0);
  return debugTerminal("HC05_getConnectionMode");
  processConnectionMode(connection_mode, 1);

  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+CMOD:");
  const char *mode_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_CMODE);

  if (m_errCode != HC05_OK)
    return false;

  if (!mode_part)
    return hc05.readOperationResult() && false;

  connection_mode = static_cast<HC05_Connection>(atol(mode_part));

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setConnectionMode
*FUNCTION     :setConnectionMode
*INPUT        :connection_mode , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setConnectionMode(
  HC05_Connection connection_mode, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CMODE=%u", connection_mode);

  //PGM_STRING_MAPPED_TO_RAM(command, "CMODE=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setConnectionMode");
}

/****************************************************************
*FUNCTION NAME:bind
*FUNCTION     :bind
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::bind(const BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "BIND");
  return hc05.writeAddressWithCommand(address, AT_BIND, timeout);
}

/****************************************************************
*FUNCTION NAME:getAddressBound
*FUNCTION     :getAddressBound
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getAddressBound(BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "BIND");
  return hc05.readAddressWithCommand(address, AT_BIND, timeout);
}

/****************************************************************
*FUNCTION NAME:getLeds
*FUNCTION     :get Leds status
*INPUT        :led_status , led_connection , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getLeds(bool &led_status,
  bool &led_connection, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "POLAR?");
  hc05.sendAtCommand("AT+POLAR?\r\n" ,timeout ,0 );
  return debugTerminal("HC05_getLeds");
  processLed(led_status, led_connection, 1);
/* 
  led_status = 0;
  led_connection = 0;

  char response[30];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+POLAR:");
  
  char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_POLAR);

  if (m_errCode != HC05_OK)
    return false;

  if (!status_part)
    return hc05.readOperationResult() && false;

  led_status = atol(status_part);
  status_part = strchrnul(status_part, ',');

  if (*status_part != ',')
    return hc05.readOperationResult() && false;

  led_connection = atol(++status_part);

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setLeds
*FUNCTION     :set Leds status
*INPUT        :led_status , led_connection , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setLeds(bool led_status,
  bool led_connection, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+%d,%d",
    (led_status ? 1 : 0), (led_connection ? 1 : 0));

  //PGM_STRING_MAPPED_TO_RAM(command, "POLAR=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setLeds");
  
}

/****************************************************************
*FUNCTION NAME:setPortState
*FUNCTION     :set HC05 Ports States
*INPUT        :port_states , port_num , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setPortState(uint8_t port_num,
  uint8_t port_state, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+PIO=%u,%u", port_num, port_state);

  //PGM_STRING_MAPPED_TO_RAM(command, "PIO=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setPortState");
  processPortState(port_num, port_state, 1);
}

/****************************************************************
*FUNCTION NAME:getMultiplePorts
*FUNCTION     :getMultiplePorts
*INPUT        :port_states , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getMultiplePorts(uint16_t &port_states, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "MPIO?");
  hc05.sendAtCommand("AT+MPIO?\r\n", timeout, 0 );
  return debugTerminal("HC05_getMultiplePorts");
  processMultiPortState(port_states, 1);

  port_states = 0;

  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+MPIO:");
  const char *states_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_MPIO);

  if (m_errCode != HC05_OK)
    return false;

  if (!states_part)
    return hc05.readOperationResult() && false;

  port_states = htoul(states_part);

  return hc05.readOperationResult();
}

/****************************************************************
*FUNCTION NAME:setMultiplePorts
*FUNCTION     :setMultiplePorts  
*INPUT        :port_states , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setMultiplePorts(uint16_t port_states, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+MPIO=%x", port_states);
  //PGM_STRING_MAPPED_TO_RAM(command, "MPIO=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setMultiplePorts");
  
}

/****************************************************************
*FUNCTION NAME:getInquiryAndPagingParams
*FUNCTION     :getInquiryAndPagingParams
*INPUT        :inquiry_interval , inquiry_duration , paging_interval , paging_duration , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getInquiryAndPagingParams(uint16_t &inquiry_interval, uint16_t &inquiry_duration, uint16_t &paging_interval, uint16_t &paging_duration, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "IPSCAN?");
  hc05.sendAtCommand("AT+IPSCAN?\r\n", timeout, 0);
  return debugTerminal("HC05_getInquiryAndPagingParams");
  processGetInquiryAndPagingParams(inquiry_interval, inquiry_duration, paging_interval, paging_duration, 1);

  char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+IPSCAN:");
  /* 
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_IPSCAN);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  inquiry_interval = atol(params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  inquiry_duration = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  paging_interval = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  paging_duration = atol(++params_part);

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setInquiryAndPagingParams
*FUNCTION     :setInquiryAndPagingParams
*INPUT        :inquiry_interval , inquiry_duration , paging_interval , paging_duration , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setInquiryAndPagingParams(
  uint16_t inquiry_interval, uint16_t inquiry_duration,
  uint16_t paging_interval, uint16_t paging_duration, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+IPSCAN=%u,%u,%u,%u",
    inquiry_interval, inquiry_duration, paging_interval, paging_duration);

  //PGM_STRING_MAPPED_TO_RAM(command, "IPSCAN=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setInquiryAndPagingParams");
}

/****************************************************************
*FUNCTION NAME:getSniffParams
*FUNCTION     :getSniffParams
*INPUT        :max_time , min_time , retry_interval , sniff_timeout , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSniffParams(uint16_t &max_time, uint16_t &min_time,
  uint16_t &retry_interval, uint16_t &sniff_timeout, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "SNIFF?");
  hc05.sendAtCommand("AT+SNIFF?\r\n", timeout, 0 );
   debugTerminal("HC05_setInquiryAndPagingParams");
  return processGetSniffParams(max_time, min_time, retry_interval, sniff_timeout, 1);
/* 
  char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+SNIFF:");
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_SNIFF);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  max_time = atol(params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  min_time = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  retry_interval = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  sniff_timeout = atol(++params_part);

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setSniffParams
*FUNCTION     :set Sniff Parameter
*INPUT        :max_time , min_time , retry_interval , sniff_timeout , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+SNIFF=%u,%u,%u,%u",
    max_time, min_time, retry_interval, sniff_timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "SNIFF=");
  hc05.sendAtCommand(temp, timeout, 0);
  return debugTerminal("HC05_setInquiryAndPagingParams");
}

/****************************************************************
*FUNCTION NAME:enterSniffMode
*FUNCTION     :enterSniffMode
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::enterSniffMode(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "ENSNIFF");
  hc05.sendAtCommand("AT+ENSNIFF", timeout, 0);
  return  debugTerminal("HC05_enterSniffMode");
}

/****************************************************************
*FUNCTION NAME:exitSniffMode
*FUNCTION     :exitSniffMode
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::exitSniffMode(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "EXSNIFF");
  hc05.sendAtCommand("AT+EXSNIFF", timeout, 0);
  return  debugTerminal("HC05_exitSniffMode");
}

/****************************************************************
*FUNCTION NAME:getSecurityAndEncryption
*FUNCTION     :getSecurityAndEncryption 
*INPUT        :Security,encryption , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getSecurityAndEncryption(HC05_Security &security,
  HC05_Encryption &encryption, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "SENM?");
  hc05.sendAtCommand("AT+SENM?\r\n", timeout, 0);
  debugTerminal("HC05_getSecurityAndEncryption");
  return processSecurityAndEncryption(security, encryption, 1);

  /* char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+SENM:");
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_SENM);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  security = static_cast<HC05_Security>(atol(params_part));
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  encryption = static_cast<HC05_Encryption>(atol(++params_part));

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:setSecurityAndEncryption
*FUNCTION     :setSecurityAndEncryption
*INPUT        :security , encryption , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSecurityAndEncryption(HC05_Security security,
  HC05_Encryption encryption, unsigned long timeout)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+SENM=%u,%u", security, encryption);

  //PGM_STRING_MAPPED_TO_RAM(command, "SENM=");
  return hc05.sendAtCommand(temp, timeout, 0);
  debugTerminal("HC05_setSecurityAndEncryption");
}


/****************************************************************
*FUNCTION NAME:deleteDeviceFromList
*FUNCTION     :deleteDeviceFromList
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::deleteDeviceFromList(
    const BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "RMSAD");
  return hc05.writeAddressWithCommand(address, AT_RMSAD, timeout);
}

/****************************************************************
*FUNCTION NAME:deleteAllDevicesFromList
*FUNCTION     :deleteAllDevicesFromList
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::deleteAllDevicesFromList(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "RMAAD");
  hc05.sendAtCommand("AT+RMAAD", timeout, 0);

  debugTerminal("HC05_deleteAllDevicesFromList");
  return processDeleteAllDevicesFromList(1);
}

/****************************************************************
*FUNCTION NAME:findDeviceInList
*FUNCTION     :findDeviceInList 
*INPUT        :address : Bluetoothaddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::findDeviceInList(
  const BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "FSAD");
  return hc05.writeAddressWithCommand(address, AT_FSAD, timeout);
}

/****************************************************************
*FUNCTION NAME:countDevicesInList
*FUNCTION     :countDevicesInList  
*INPUT        :device_count , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::countDevicesInList(uint8_t &device_count, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "ADCN?");
  hc05.sendAtCommand("AT+ADCN?\r\n", timeout, 0);
  debugTerminal("HC05_countDevicesInList");
  return processCountDevicesInList(device_count, 1);
 /*  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+ADCN:");
  const char *count_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_ADCN);

  if (m_errCode != HC05_OK)
    return false;

  if (!count_part)
    return hc05.readOperationResult() && false;

  device_count = atol(count_part);

  return hc05.readOperationResult();
 */}

/****************************************************************
*FUNCTION NAME:getLastAuthenticatedDevice
*FUNCTION     :getLastAuthenticatedDevice  
*INPUT        :address : BluetoothAddress , timeout;
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getLastAuthenticatedDevice(
  BluetoothAddress &address, unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command_name, "MRAD");
  return hc05.readAddressWithCommand(address, AT_MRAD, timeout);
}

/****************************************************************
*FUNCTION NAME:getState
*FUNCTION     :getState
*INPUT        :state , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getState(HC05_State &state, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "STATE?");
  hc05.sendAtCommand("AT+STATE?\r\n", timeout, 0);
  debugTerminal("HC05_getState");
 

  
return processState(state, 1);
  /* char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+STATE:");
  const char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), "+STATE:");

  if (m_errCode != HC05_OK)
    return false;

  if (!status_part)
    return hc05.readOperationResult() && false; */

  //PGM_STRING_MAPPED_TO_RAM(INITIALIZED, "INITIALIZED");
  //PGM_STRING_MAPPED_TO_RAM(READY, "READY");
  //PGM_STRING_MAPPED_TO_RAM(PAIRABLE, "PAIRABLE");
  //PGM_STRING_MAPPED_TO_RAM(PAIRED, "PAIRED");
  //PGM_STRING_MAPPED_TO_RAM(INQUIRING, "INQUIRING");
  //PGM_STRING_MAPPED_TO_RAM(CONNECTING, "CONNECTING");
  //PGM_STRING_MAPPED_TO_RAM(CONNECTED, "CONNECTED");
  //PGM_STRING_MAPPED_TO_RAM(DISCONNECTED, "DISCONNECTED");
  //PGM_STRING_MAPPED_TO_RAM(UNKNOWN, "UNKNOWN");

  /* if (strcmp(status_part, AT_INITIALIZED) == 0)
    state = HC05_INITIALIZED;
  else if (strcmp(status_part, AT_READY) == 0)
    state = HC05_READY;
  else if (strcmp(status_part, AT_PAIRABLE) == 0)
    state = HC05_PAIRABLE;
  else if (strcmp(status_part, AT_PAIRED) == 0)
    state = HC05_PAIRED;
  else if (strcmp(status_part, AT_INQUIRING) == 0)
    state = HC05_INQUIRING;
  else if (strcmp(status_part, AT_CONNECTING) == 0)
    state = HC05_CONNECTING;
  else if (strcmp(status_part, AT_CONNECTED) == 0)
    state = HC05_CONNECTED;
  else if (strcmp(status_part, AT_DISCONNECTED) == 0)
    state = HC05_DISCONNECTED;
  else if (strcmp(status_part, AT_UNKNOWN) == 0)
    state = HC05_UNKNOWN;

  return hc05.readOperationResult(); */
}

/****************************************************************
*FUNCTION NAME:initSerialPortProfile
*FUNCTION     :initSerialPortProfile  
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::initSerialPortProfile(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "INIT");
  hc05.sendAtCommand("AT+INIT", timeout, 0);
  return debugTerminal("HC05_initSerialPortProfile");
}

/****************************************************************
*FUNCTION NAME:inquire
*FUNCTION     :inquire
*INPUT        :callback , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::inquire(InquiryCallback callback, unsigned long timeout)
{
  startOperation(timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "INQ");
  hc05.sendAtCommand("AT+INQ\r\n", timeout, 0);
  return debugTerminal("HC05_inquire");

  while (!isOperationTimedOut())
  {
    //if (m_uart->peek() != '+')
    if(USART_ReceiveData(HC05_USART_CH) != '+')
      break;

    char response[HC05_ADDRESS_BUFSIZE + 10];
    
    //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+INQ:");
    const char *address_part;
    address_part = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_INQ);

    BluetoothAddress address;
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
bool LIB_HC05::cancelInquiry(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "INQC");
  return hc05.sendAtCommand(AT_INQC, 0, timeout);
  return debugTerminal("HC05_cancelInquiry");
}

/****************************************************************
*FUNCTION NAME:pair
*FUNCTION     :pairing AT commands  
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::pair(const BluetoothAddress &address, unsigned long timeout)
{
  char params_str[HC05_ADDRESS_BUFSIZE + 15];
  int address_length = hc05.printBluetoothAddress(params_str, address, ',');

  //PGM_STRING_MAPPED_TO_RAM(format, ",%lu");
  snprintf(params_str + address_length,
    sizeof(params_str) - address_length, "AT+PAIR,%lu", timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "PAIR");
  hc05.sendAtCommand(params_str, timeout, 0);
  return debugTerminal("HC05_pair");
  processPair(1);
}

/****************************************************************
*FUNCTION NAME:connect
*FUNCTION     :connect  
*INPUT        :address : BluetoothAddress , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::connect(const BluetoothAddress &address, unsigned long timeout)
{
  return hc05.writeAddressWithCommand(address, "LINK", timeout);
}

/****************************************************************
*FUNCTION NAME:disconnect
*FUNCTION     :disconnect commands
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::disconnect(unsigned long timeout)
{
  startOperation(timeout);
  hc05.sendAtCommand("AT+DISC\r\n", timeout, 0 );
  return debugTerminal("HC05_pair");
  processDisconnect(1);

  //PGM_STRING_MAPPED_TO_RAM(SUCCESS, "SUCCESS");
  //PGM_STRING_MAPPED_TO_RAM(LINK_LOSS, "LINK_LOSS");
  //PGM_STRING_MAPPED_TO_RAM(NO_SLC, "NO_SLC");
 // PGM_STRING_MAPPED_TO_RAM(TIMEOUT, "TIMEOUT");
  //PGM_STRING_MAPPED_TO_RAM(ERROR, "ERROR");

  char response[20];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, AT_RESP_DISC);
  
  const char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_DISC);
/* 
  if (strcmp(status_part, AT_SUCCESS) == 0)
    m_errCode = HC05_OK;
  else if (strcmp(status_part, AT_LINK_LOSS) == 0)
    m_errCode = HC05_ERR_DISC_LINK_LOSS;
  else if (strcmp(status_part, AT_NO_SLC) == 0)
    m_errCode = HC05_ERR_DISC_NO_SLC;
  else if (strcmp(status_part, AT_TIMEOUT) == 0)
    m_errCode = HC05_ERR_DISC_TIMEOUT;
  else if (strcmp(status_part, AT_ERROR) == 0)
    m_errCode = HC05_ERR_DISC_ERROR;

 */   return hc05.readOperationResult();
}



/****************************************************************
*FUNCTION NAME:processversion
*FUNCTION     :processversion
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processVersion(char *buffer, size_t buffer_size, uint16_t addrs)
{
	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"+VERSION:");
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_getversion");
  
  
	
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
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  

    char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+RNAME:");
  char *name_part = hc05.readResponseWithPrefix(response, sizeof(response), "+RNAME:");

  if (m_errCode != HC05_OK)
    return false;

  if (!name_part)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }

  //PGM_STRING_MAPPED_TO_RAM(format, "%s");
  snprintf(buffer, buffer_size, "%s", name_part);

     
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_RemoteDeviceName");
  
  

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
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  device_class = 0;

  char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+CLASS:");
  const char *class_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_CLASS);

  if (m_errCode != HC05_OK)
    return false;

  if (!class_part)
    return hc05.readOperationResult() && false;

  device_class = htoul(class_part);

  return hc05.readOperationResult();

  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_DeviceClass");
  
  

}

/****************************************************************
*FUNCTION NAME:process
*FUNCTION     :process
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processInquiryAccessCode( uint32_t &iac, uint16_t addrs)
{

  iac = 0;
   debugTerminal("HC05_InquiryAccessCode");

  if (isOperationTimedOut())
    return false;

  char response[30];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+IAC:");
  const char *iac_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_IAC);

  if (m_errCode != HC05_OK)
    return false;

  if (!iac_part)
    return hc05.readOperationResult() && false;

  iac = htoul(iac_part);

  return hc05.readOperationResult();

  
  

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

  inq_mode = HC05_INQUIRY_STANDARD;
  max_devices = 0;
  max_duration = 0;

  char response[30];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+INQM:");
  char *mode_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_INQM);

  if (m_errCode != HC05_OK)
    return false;

  if (!mode_part)
    return hc05.readOperationResult() && false;

  inq_mode = static_cast<HC05_InquiryMode>(atol(mode_part));
  mode_part = strchrnul(mode_part, ',');

  if (*mode_part != ',')
    return hc05.readOperationResult() && false;

  max_devices = atol(++mode_part);
  mode_part = strchrnul(mode_part, ',');

  if (*mode_part != ',')
    return hc05.readOperationResult() && false;

  max_duration = atol(++mode_part);

  return hc05.readOperationResult();

 debugTerminal("HC05_InquiryMode");
  
  

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
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_Password");
  
  

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
	
  char response[30];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+UART:");
  char *mode_str = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_UART);

  if (m_errCode != HC05_OK)
    return false;

  if (!mode_str)
    return hc05.readOperationResult() && false;

  speed = atol(mode_str);
  mode_str = strchrnul(mode_str, ',');

  if (*mode_str != ',')
  {
    m_errCode = HC05_FAIL;
    return hc05.readOperationResult() && false;
  }

  stop_bits = atol(++mode_str) + 1;
  mode_str = strchrnul(mode_str, ',');

  if (*mode_str != ',')
  {
    m_errCode = HC05_FAIL;
    return hc05.readOperationResult() && false;
  }

  parity = static_cast<HC05_Parity>(atol(++mode_str));

  return hc05.readOperationResult();

  // update Flash IC here
		
   debugTerminal("HC05_SerialMode");
  
  

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
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_ConnectionMode");
  
  

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


  // update Flash IC here
		
   debugTerminal("HC05_getLedStatus");
  
  led_status = 0;
  led_connection = 0;

  char response[30];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+POLAR:");
  
  char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_POLAR);

  if (m_errCode != HC05_OK)
    return false;

  if (!status_part)
    return hc05.readOperationResult() && false;

  led_status = atol(status_part);
  status_part = strchrnul(status_part, ',');

  if (*status_part != ',')
    return hc05.readOperationResult() && false;

  led_connection = atol(++status_part);

  return hc05.readOperationResult();

  

}

/****************************************************************
*FUNCTION NAME:processPortState
*FUNCTION     :processPortState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processPortState(uint8_t port_num,
  uint8_t port_state, uint16_t addrs)
{
	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_PortState");
  
  

}

/****************************************************************
*FUNCTION NAME:processMultiPortState
*FUNCTION     :processMultiPortState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processMultiPortState(uint16_t &port_states, uint16_t addrs)
{
  char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];
        
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("HC05_processMultiPortState");
  
  port_states = 0;

  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+MPIO:");
  const char *states_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_MPIO);

  if (m_errCode != HC05_OK)
    return false;

  if (!states_part)
    return hc05.readOperationResult() && false;

  port_states = htoul(states_part);

  return hc05.readOperationResult();
  

}

/****************************************************************
*FUNCTION NAME:processGetInquiryAndPagingParams
*FUNCTION     :processGetInquiryAndPagingParams
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processGetInquiryAndPagingParams(uint16_t &inquiry_interval, 
uint16_t &inquiry_duration, uint16_t &paging_interval, uint16_t &paging_duration, uint16_t addrs)
{

	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  debugTerminal("HC05_processGetInquiryAndPagingParams");
  char response[40];
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_IPSCAN);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  inquiry_interval = atol(params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  inquiry_duration = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  paging_interval = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  paging_duration = atol(++params_part);

  return hc05.readOperationResult();
 

  // update Flash IC here
		
  
  

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
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+SENM:");
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_SENM);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  security = static_cast<HC05_Security>(atol(params_part));
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  encryption = static_cast<HC05_Encryption>(atol(++params_part));

  return hc05.readOperationResult();
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("HC05_processSecurityAndEncryption");
  
  

}

/****************************************************************
*FUNCTION NAME:processState
*FUNCTION     :processState
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processState(HC05_State &state, uint16_t addrs)
{
	char      *strStart, str1[40];
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+STATE:");
  const char *status_part = hc05.readResponseWithPrefix(
    str1, sizeof(str1), "+STATE:");

  if (m_errCode != HC05_OK)
    return false;

  if (!status_part)
    return hc05.readOperationResult() && false;
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
	state = HC05_UNKNOWN;
  debugTerminal("HC05_processState");
  
  if (strcmp(status_part, AT_INITIALIZED) == 0)
    state = HC05_INITIALIZED;
  else if (strcmp(status_part, AT_READY) == 0)
    state = HC05_READY;
  else if (strcmp(status_part, AT_PAIRABLE) == 0)
    state = HC05_PAIRABLE;
  else if (strcmp(status_part, AT_PAIRED) == 0)
    state = HC05_PAIRED;
  else if (strcmp(status_part, AT_INQUIRING) == 0)
    state = HC05_INQUIRING;
  else if (strcmp(status_part, AT_CONNECTING) == 0)
    state = HC05_CONNECTING;
  else if (strcmp(status_part, AT_CONNECTED) == 0)
    state = HC05_CONNECTED;
  else if (strcmp(status_part, AT_DISCONNECTED) == 0)
    state = HC05_DISCONNECTED;
  else if (strcmp(status_part, AT_UNKNOWN) == 0)
    state = HC05_UNKNOWN;

  return hc05.readOperationResult();

}

/****************************************************************
*FUNCTION NAME:processPair
*FUNCTION     :processPair
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processPair(uint16_t addrs)
{

	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("HC05_HC05_processPair");
  
  

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processDisconnect(uint16_t addrs)
{

	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  
  if(str1!=NULL)
    sscanf(str1,"%s",Sim80x.IMEI);

  // update Flash IC here
	
  const char *status_part = hc05.readResponseWithPrefix(
    Sim80x.IMEI, sizeof(Sim80x.IMEI), "+DISC:");
  if (strcmp(status_part, "SUCCESS") == 0)
    m_errCode = HC05_OK;
  else if (strcmp(status_part, "LINK_LOSS") == 0)
    m_errCode = HC05_ERR_DISC_LINK_LOSS;
  else if (strcmp(status_part, "NO_SLC") == 0)
    m_errCode = HC05_ERR_DISC_NO_SLC;
  else if (strcmp(status_part, "TIMEOUT") == 0)
    m_errCode = HC05_ERR_DISC_TIMEOUT;
  else if (strcmp(status_part, "ERROR") == 0)
    m_errCode = HC05_ERR_DISC_ERROR;

	
  debugTerminal("HC05_HC05_processDisconnect");
  
  

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processCountDevicesInList(uint8_t &device_count, uint16_t addrs)

{
  char response[20];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+ADCN:");
  const char *count_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_ADCN);

  if (m_errCode != HC05_OK)
    return false;

  if (!count_part)
    return hc05.readOperationResult() && false;

  device_count = atol(count_part);

  return hc05.readOperationResult();

}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processDeleteAllDevicesFromList(uint16_t addrs)
{	
	char      *strStart, *str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"A/\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"A/\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  return debugTerminal("Sim80x_getLastCommand");
  
}

/****************************************************************
*FUNCTION NAME:processDisconnect
*FUNCTION     :processDisconnect
*INPUT        :addrs
*OUTPUT       :void
****************************************************************/
bool LIB_HC05::processGetSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, uint16_t addrs)
{
  char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+SNIFF:");
  char *params_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_SNIFF);

  if (m_errCode != HC05_OK)
    return false;

  if (!params_part)
    return hc05.readOperationResult() && false;

  max_time = atol(params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  min_time = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  retry_interval = atol(++params_part);
  params_part = strchrnul(params_part, ',');

  if (*params_part != ',')
    return hc05.readOperationResult() && false;

  sniff_timeout = atol(++params_part);

  return hc05.readOperationResult();
 
}







#endif

#endif