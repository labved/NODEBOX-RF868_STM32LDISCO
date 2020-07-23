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
  char      *strStart,*str1;
  setAtMode();
  
  hc05.sendAtCommand("AT+VERSION?\r\n", 200, 0);
  
  // Read & Update Operation
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  

  str1 = strstr(strStart,"+VERSION:");
  
  if(str1!=NULL)
    sscanf(str1,"+VERSION:%s",Sim80x.IMEI);    

  debugTerminal("HC05_getVersion");

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
*FUNCTION NAME:debugTerminal
*FUNCTION     :Prints response to terminal IO
*INPUT        :message
*OUTPUT       :void
****************************************************************/
void LIB_HC05::debugTerminal(char *msg)
{
  #if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
     char      *strStart,*str1;

     strStart = (char*)&Sim80x.UsartRxBuffer[0];  
      str1 = strstr(strStart,"\r\nOK\r\n");

      if(str1!=NULL)
      {
        #if (_SIM80X_DEBUG==2)
          printf("\r\nAT Respone : %s",Sim80x.UsartRxBuffer);
        #endif
        printf("\r\n%s ---> OK\r\n", msg);
      }
      else
        printf("\r\n% ---> FAILED\r\n", msg);

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
bool LIB_HC05::softReset(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(reset_cmd, "RESET");
  return (hc05.simpleCommand(AT_RESET, 0, timeout));
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
  hc05.writeCommand(0);
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
  return hc05.sendAtCommand("AT+ORGL\r\n", 200, 0);
}

/****************************************************************
*FUNCTION NAME:getVersion
*FUNCTION     :get version number 
*INPUT        :buffer , buffer_size , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::getVersion(char *buffer, size_t buffer_size, unsigned long timeout)
{
  startOperation(timeout);

  if (!buffer || buffer_size <= 1)
    return false;

  //PGM_STRING_MAPPED_TO_RAM(command, "VERSION?");
  hc05.writeCommand(AT_CMD_GET_VERSION);

  /* Response should look like "+VERSION:2.0-20100601" */
  char response[30];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+VERSION:");
  
  const char *version = hc05.readResponseWithPrefix(
     response, sizeof(response), AT_RESP_VERSION);
 
  if (m_errCode != HC05_OK)
    return false;

  if (!version)
  {
    *buffer = 0;
    return hc05.readOperationResult() && false;
  }

  snprintf(buffer, buffer_size, "%s", version);
  return hc05.readOperationResult();
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
  hc05.writeCommand(AT_CMD_GET_NAME);

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
  //PGM_STRING_MAPPED_TO_RAM(command, "NAME=");
  return(hc05.simpleCommand(AT_CMD_SET_NAME, name, timeout));
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
  
  hc05.writeCommand(AT_CMD_GET_RNAME, address_str);

  char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+RNAME:");
  char *name_part = hc05.readResponseWithPrefix(response, sizeof(response), AT_RESP_RNAME);

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
  hc05.writeCommand(AT_CMD_GET_ROLE);

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
}

/****************************************************************
*FUNCTION NAME:setRole
*FUNCTION     :setRole
*INPUT        :role , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setRole(HC05_Role role, unsigned long timeout)
{
  char role_str[10] = { role, 0 };
 // PGM_STRING_MAPPED_TO_RAM(format, "%d");
  snprintf(role_str, sizeof(role_str), "%d", role);

  //PGM_STRING_MAPPED_TO_RAM(command, "ROLE=");
  return hc05.simpleCommand(AT_CMD_SET_ROLE, role_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_CLASS);

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
}

/****************************************************************
*FUNCTION NAME:setDeviceClass
*FUNCTION     :set Device Class
*INPUT        :device_class , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setDeviceClass(uint32_t device_class, unsigned long timeout)
{
  char class_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%lx");
  snprintf(class_str, sizeof(class_str), "%lx", device_class);

 // PGM_STRING_MAPPED_TO_RAM(command, "CLASS=");
  return hc05.simpleCommand(AT_CMD_SET_CLASS, class_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_IAC);

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
  char iac_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%lx");
  snprintf(iac_str, sizeof(iac_str), "%lx", iac);

  //PGM_STRING_MAPPED_TO_RAM(command, "IAC=");
  return hc05.simpleCommand(AT_CMD_SET_IAC, iac_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_INQM);

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
  snprintf(mode, sizeof(mode), "%d,%u,%u", inq_mode, (uint16_t)max_devices, max_duration);

  //PGM_STRING_MAPPED_TO_RAM(command, "INQM=");
  return hc05.simpleCommand(AT_CMD_SET_INQM, mode, timeout);
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
  hc05.writeCommand(AT_CMD_GET_PSWD);

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
  //PGM_STRING_MAPPED_TO_RAM(command, "PSWD=");
  return hc05.simpleCommand(AT_CMD_SET_PSWD, password, timeout);
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
  hc05.writeCommand(AT_CMD_GET_UART);

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

  char mode_str[20];
  //PGM_STRING_MAPPED_TO_RAM(format, "%lu,%u,%u");
  snprintf(mode_str, sizeof(mode_str), "%lu,%u,%u", speed, stop_bits, parity);

  //PGM_STRING_MAPPED_TO_RAM(command, "UART=");
  return hc05.simpleCommand(AT_CMD_SET_UART, mode_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_CMODE);

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
  char mode_str[20];
  
  //PGM_STRING_MAPPED_TO_RAM(format, "%u");
  snprintf(mode_str, sizeof(mode_str), "%u", connection_mode);

  //PGM_STRING_MAPPED_TO_RAM(command, "CMODE=");
  return hc05.simpleCommand(AT_CMD_SET_CMODE, mode_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_POLAR);

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
*FUNCTION NAME:setLeds
*FUNCTION     :set Leds status
*INPUT        :led_status , led_connection , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setLeds(bool led_status,
  bool led_connection, unsigned long timeout)
{
  char leds_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%d,%d");
  snprintf(leds_str, sizeof(leds_str), "%d,%d",
    (led_status ? 1 : 0), (led_connection ? 1 : 0));

  //PGM_STRING_MAPPED_TO_RAM(command, "POLAR=");
  return hc05.simpleCommand(AT_CMD_SET_POLAR, leds_str, timeout);
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
  char state_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%u,%u");
  snprintf(state_str, sizeof(state_str), "%u,%u", port_num, port_state);

  //PGM_STRING_MAPPED_TO_RAM(command, "PIO=");
  return hc05.simpleCommand(AT_CMD_SET_PIO, state_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_MPIO);

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
  char states_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%x");
  snprintf(states_str, sizeof(states_str), "%x", port_states);

  //PGM_STRING_MAPPED_TO_RAM(command, "MPIO=");
  return hc05.simpleCommand(AT_CMD_SET_MPIO, states_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_IPSCAN);

  char response[40];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+IPSCAN:");
  
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
}

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
  char params_str[40];
  
  //PGM_STRING_MAPPED_TO_RAM(format, "%u,%u,%u,%u");
  snprintf(params_str, sizeof(params_str), "%u,%u,%u,%u",
    inquiry_interval, inquiry_duration, paging_interval, paging_duration);

  //PGM_STRING_MAPPED_TO_RAM(command, "IPSCAN=");
  return hc05.simpleCommand(AT_CMD_SET_IPSCAN, params_str, timeout);
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
  hc05.writeCommand(AT_CMD_GET_SNIFF);

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

/****************************************************************
*FUNCTION NAME:setSniffParams
*FUNCTION     :set Sniff Parameter
*INPUT        :max_time , min_time , retry_interval , sniff_timeout , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, unsigned long timeout)
{
  char params_str[40];
  //PGM_STRING_MAPPED_TO_RAM(format, "%u,%u,%u,%u");
  snprintf(params_str, sizeof(params_str), "%u,%u,%u,%u",
    max_time, min_time, retry_interval, sniff_timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "SNIFF=");
  return hc05.simpleCommand(AT_CMD_SET_SNIFF, params_str, timeout);
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
  return hc05.simpleCommand(AT_CMD_ENSNIFF, 0, timeout);
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
  return hc05.simpleCommand(AT_CMD_EXSNIFF, 0, timeout);
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
  hc05.writeCommand(AT_CMD_GET_SENM);

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
}

/****************************************************************
*FUNCTION NAME:setSecurityAndEncryption
*FUNCTION     :setSecurityAndEncryption
*INPUT        :security , encryption , timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::setSecurityAndEncryption(HC05_Security security,
  HC05_Encryption encryption, unsigned long timeout)
{
  char params_str[10];
  //PGM_STRING_MAPPED_TO_RAM(format, "%u,%u");
  snprintf(params_str, sizeof(params_str), "%u,%u", security, encryption);

  //PGM_STRING_MAPPED_TO_RAM(command, "SENM=");
  return hc05.simpleCommand(AT_CMD_SET_SENM, params_str, timeout);
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
  return hc05.simpleCommand(AT_RMAAD, 0, timeout);
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
  hc05.writeCommand(AT_CMD_GET_ADCN);

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
  hc05.writeCommand(AT_CMD_GET_STATE);

  state = HC05_UNKNOWN;

  char response[40];
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, "+STATE:");
  const char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_STATE);

  if (m_errCode != HC05_OK)
    return false;

  if (!status_part)
    return hc05.readOperationResult() && false;

  //PGM_STRING_MAPPED_TO_RAM(INITIALIZED, "INITIALIZED");
  //PGM_STRING_MAPPED_TO_RAM(READY, "READY");
  //PGM_STRING_MAPPED_TO_RAM(PAIRABLE, "PAIRABLE");
  //PGM_STRING_MAPPED_TO_RAM(PAIRED, "PAIRED");
  //PGM_STRING_MAPPED_TO_RAM(INQUIRING, "INQUIRING");
  //PGM_STRING_MAPPED_TO_RAM(CONNECTING, "CONNECTING");
  //PGM_STRING_MAPPED_TO_RAM(CONNECTED, "CONNECTED");
  //PGM_STRING_MAPPED_TO_RAM(DISCONNECTED, "DISCONNECTED");
  //PGM_STRING_MAPPED_TO_RAM(UNKNOWN, "UNKNOWN");

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
*FUNCTION NAME:initSerialPortProfile
*FUNCTION     :initSerialPortProfile  
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_HC05::initSerialPortProfile(unsigned long timeout)
{
  //PGM_STRING_MAPPED_TO_RAM(command, "INIT");
  return hc05.simpleCommand(AT_INIT, 0, timeout);
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
  hc05.writeCommand(AT_INQ);

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
  return hc05.simpleCommand(AT_INQC, 0, timeout);
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
    sizeof(params_str) - address_length, ",%lu", timeout);

  //PGM_STRING_MAPPED_TO_RAM(command, "PAIR");
  return hc05.simpleCommand(AT_PAIR, params_str, timeout);
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
  hc05.writeCommand(AT_DISC);

  //PGM_STRING_MAPPED_TO_RAM(SUCCESS, "SUCCESS");
  //PGM_STRING_MAPPED_TO_RAM(LINK_LOSS, "LINK_LOSS");
  //PGM_STRING_MAPPED_TO_RAM(NO_SLC, "NO_SLC");
 // PGM_STRING_MAPPED_TO_RAM(TIMEOUT, "TIMEOUT");
  //PGM_STRING_MAPPED_TO_RAM(ERROR, "ERROR");

  char response[20];
  
  //PGM_STRING_MAPPED_TO_RAM(response_pattern, AT_RESP_DISC);
  
  const char *status_part = hc05.readResponseWithPrefix(
    response, sizeof(response), AT_RESP_DISC);

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

   return hc05.readOperationResult();
}

#endif

#endif