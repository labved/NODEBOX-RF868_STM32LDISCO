#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_BT
  #define _USE_BT
#endif

#if(_USE_BT==1)
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HC05_SHORTCODES.h"
#include "HC05_TYPEDEF.h"
#include "HAL_HC05.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

//HC05_Result m_errorCode;
HC05_Result m_errCode;

#pragma location=0x1FF7FF90
__no_init char ReservedArea[256];

/*
#define PGM_STRING_MAPPED_TO_RAM(name, src) \
  static const char name##_pgm[] PROGMEM = src; \
  char name[sizeof(name##_pgm)]; \
  strcpy_P(name, name##_pgm);
*/

void PGM_STRING_MAPPED_TO_RAM(char *name,const char *src)
{
  //uint16_t size;
 
  strcpy(name, src);

};

/*
void PGM_STRING_MAPPED_TO_RAM(void *name, const char *src)   
{
  //strlen(const char *);
  uint8_t size=strlen(src);   
  
  //memcpy(void *_Restrict, const void *_Restrict, size_t);
  memcpy(ReservedArea,src,size); 
}
*/

/****************************************************************
*FUNCTION NAME:gpioInit
*FUNCTION     :Init
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_HC05::gpioInit(void)                             
{
  // Setting GPIO for STATE
  RCC_AHBPeriphClockCmd(HC05_RCC_STATE, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(HC05_STATE_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef HC05_GPIO_STATE_InitStruct;

  HC05_GPIO_STATE_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  HC05_GPIO_STATE_InitStruct.GPIO_OType = GPIO_OType_PP;
  HC05_GPIO_STATE_InitStruct.GPIO_Pin = HC05_PIN_STATE;
  HC05_GPIO_STATE_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  HC05_GPIO_STATE_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(HC05_GPIO_STATE, &HC05_GPIO_STATE_InitStruct);
  
  // Setting GPIO for EN
  RCC_AHBPeriphClockCmd(HC05_RCC_EN, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(HC05_EN_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef HC05_GPIO_EN_InitStruct;

  HC05_GPIO_EN_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  HC05_GPIO_EN_InitStruct.GPIO_OType = GPIO_OType_PP;
  HC05_GPIO_EN_InitStruct.GPIO_Pin = HC05_PIN_EN;
  HC05_GPIO_EN_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  HC05_GPIO_EN_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(HC05_GPIO_EN, &HC05_GPIO_EN_InitStruct);
  

  // Setting GPIO for KEY
  RCC_AHBPeriphClockCmd(HC05_RCC_KEY, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(HC05_KEY_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef HC05_GPIO_KEY_InitStruct;

  HC05_GPIO_KEY_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  HC05_GPIO_KEY_InitStruct.GPIO_OType = GPIO_OType_PP;
  HC05_GPIO_KEY_InitStruct.GPIO_Pin = HC05_PIN_KEY;
  HC05_GPIO_KEY_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  HC05_GPIO_KEY_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(HC05_GPIO_KEY, &HC05_GPIO_KEY_InitStruct);
  
   GPIO_ResetBits(HC05_GPIO_EN, HC05_PIN_EN);      //  EN pulled to LOW
   delay_ms(100);
  
   GPIO_SetBits(HC05_GPIO_EN, HC05_PIN_EN);       // EN floating to HIGH
   delay_ms(1000);   
}
/****************************************************************
*FUNCTION NAME:uartInit
*FUNCTION     :initialise the UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_HC05::uartInit(uint16_t baudrate)                             
{
  
  RCC_AHBPeriphClockCmd(HC05_RCC_TX, ENABLE); // Enable GPIO clock  
  RCC_AHBPeriphClockCmd(HC05_RCC_RX, ENABLE); // Enable GPIO clock  

  RCC_APB2PeriphClockCmd(HC05_RCC_APB_UART, ENABLE); // Enable USART clock

  // GPIO Setting for TX RX
  GPIO_InitTypeDef   HC05_GPIO_InitStruct;

  HC05_GPIO_InitStruct.GPIO_Pin = HC05_PIN_TX | HC05_PIN_RX;
  HC05_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  HC05_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  HC05_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  HC05_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  
  GPIO_Init(HC05_GPIO, &HC05_GPIO_InitStruct);
  GPIO_Init(HC05_GPIO, &HC05_GPIO_InitStruct);
  
  GPIO_PinAFConfig(HC05_GPIO, HC05_AF_PIN_TX, HC05_GPIO_AF);
  GPIO_PinAFConfig(HC05_GPIO, HC05_AF_PIN_RX, HC05_GPIO_AF);
    
  // UART Setting
  USART_DeInit(HC05_USART_CH);
  
  USART_InitTypeDef  HC05_USART_InitStruct;        

  HC05_USART_InitStruct.USART_BaudRate = baudrate;
  HC05_USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  HC05_USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  HC05_USART_InitStruct.USART_Parity = USART_Parity_No;
  HC05_USART_InitStruct.USART_StopBits = USART_StopBits_1;
  HC05_USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  
  USART_Init(HC05_USART_CH, &HC05_USART_InitStruct);
  
  USART_Cmd(HC05_USART_CH, ENABLE);
   
    /*
  //NVIC_InitTypeDef   NVIC_InitStruct;

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStruct);
  
  */
 
}
/****************************************************************
*FUNCTION NAME:hardReset
*FUNCTION     :Reset Pin
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_HC05::hardReset(void)
{
  GPIO_ResetBits(HC05_GPIO_EN, HC05_PIN_EN);      //  EN pulled to LOW
  delay_ms(100);
  GPIO_SetBits(HC05_GPIO_EN, HC05_PIN_EN);        // EN floating to HIGH
  delay_ms(1000);
  
  m_errCode = HC05_OK;
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::set2Tx(void)
{
  req2Rx();
  setTxMode();
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::set2Rx(void)
{
  req2Tx();
  setRxMode();
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::req2Tx(void)
{
  putc(XON_BYTE);
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::req2Rx(void)
{
  putc(XOFF_BYTE);
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::setTxMode(void)
{
  //CP2102TxFlag = 1;
  //CP2102RxFlag = 0;
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::setRxMode(void)
{
  //CP2102TxFlag = 0;
  //CP2102RxFlag = 1 ; 
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::flowControl(void)
{
  byte temp;
   
  temp = getc();
  
  if(temp == XOFF_BYTE)
  {
    setRxMode();
  } 
  else if(temp == XON_BYTE)
  {
    setTxMode();
  }
}
/****************************************************************
*FUNCTION NAME:write
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void  HAL_HC05::putc(byte ch)                        
{
  while(!(USART_GetFlagStatus(HC05_USART_CH, USART_FLAG_TXE)));

  USART_SendData(HC05_USART_CH, ch);
  //delay_us(500);
}
/****************************************************************
*FUNCTION NAME:write
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
byte  HAL_HC05::getc(void)                            
{
  while (!(USART_GetFlagStatus(HC05_USART_CH, USART_FLAG_RXNE)));

  return USART_ReceiveData(HC05_USART_CH);
}
/****************************************************************
*FUNCTION NAME:HAL_UART_Transmit
*FUNCTION     :HAL_UART_Transmit
*INPUT        :USARTx,str,size, timeout;
*OUTPUT       :none
****************************************************************/
void HAL_HC05::HAL_UART_Transmit(USART_TypeDef* USARTx,uint8_t *str,
uint8_t size,uint8_t timeout)
{
  char temp;
  for(uint8_t i=0;i<size;i++)
  {
      temp = *str;
      putc(temp);
      str++;
  }

}
/****************************************************************
*FUNCTION NAME:write8
*FUNCTION     :write8
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
void  HAL_HC05::write8(uint8_t *data) // ASK BHARAT FOR CONFIRMATION
{
  USART_ITConfig(LORA_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,Data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(LORA_USART_CH, data, 1,100);
    //delay_ms(10);
  #endif
}
/****************************************************************
*FUNCTION NAME:writeRaw
*FUNCTION     :writeRaw
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
void  HAL_HC05::writeRaw(uint8_t *data, uint16_t len)
{
  USART_ITConfig(LORA_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,Data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(LORA_USART_CH,data,len,100);
    //delay_ms(10);
  #endif
}

/****************************************************************
*FUNCTION NAME:coreSendAtCommand
*FUNCTION     :coreSendAtCommand
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
uint8_t  HAL_HC05::sendAtCommand(char const *AtCommand,int32_t  MaxWaiting_ms,uint8_t const HowMuchAnswers,...)
{
  while(Sim80x.Status.Busy == 1)
  {
    delay_ms(100);
  }
  Sim80x.Status.Busy = 1;
  Sim80x.AtCommand.FindAnswer = 0;
  Sim80x.AtCommand.ReceiveAnswerExeTime=0;
  //Sim80x.AtCommand.SendCommandStartTime = HAL_GetTick();
  Sim80x.AtCommand.ReceiveAnswerMaxWaiting = MaxWaiting_ms;
  
  memset(Sim80x.AtCommand.ReceiveAnswer,0,sizeof(Sim80x.AtCommand.ReceiveAnswer)); // clear buffer
  
  va_list tag;
  va_start (tag,HowMuchAnswers);
  
  char *arg[10];
  for(uint8_t i=0; i<HowMuchAnswers ; i++)
  {
    arg[i] = va_arg (tag, char *);	
    strncpy(Sim80x.AtCommand.ReceiveAnswer[i],arg[i],sizeof(Sim80x.AtCommand.ReceiveAnswer[0]));
  }
  va_end (tag);	
  
  strncpy(Sim80x.AtCommand.SendCommand,AtCommand,sizeof(Sim80x.AtCommand.SendCommand)); 
  writeString(Sim80x.AtCommand.SendCommand); 
  
  readLine(Sim80x.UsartRxBuffer, sizeof(Sim80x.UsartRxBuffer));
  
  while(MaxWaiting_ms > 0)
  {
    delay_ms(10);
    if(Sim80x.AtCommand.FindAnswer > 0)
      return Sim80x.AtCommand.FindAnswer;    
    MaxWaiting_ms-=10;
  }
  memset(Sim80x.AtCommand.ReceiveAnswer,0,sizeof(Sim80x.AtCommand.ReceiveAnswer));
  
  Sim80x.Status.Busy=0;
  return Sim80x.AtCommand.FindAnswer;
}
/****************************************************************
*FUNCTION NAME:writeString
*FUNCTION     :write string to UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_HC05::writeString(char const *data)
{
  uint16_t size = strlen(data);
  
  //m_uart->print(data);
  
  for(uint16_t i=0;i<size;i++)
  {
    putc(data[i]);  // CHANGE TO write8
  }

}


/****************************************************************
*FUNCTION NAME:writeCommand
*FUNCTION     :write AT commands
*INPUT        :command : AT commands, arg : AT arguments
*OUTPUT       :none
****************************************************************/
void HAL_HC05::writeCommand(const char *command, const char *arg)
{
  //PGM_STRING_MAPPED_TO_RAM(AT, "AT");
  //m_uart->print(AT);
  writeString("AT");

  if (command && command[0] != 0)
  {
    //m_uart->write('+');
    putc('+');    // CHANGE TO write8
    
    //m_uart->print(command);
    writeString(command); 
  }

  if (arg && arg[0] != 0)
  {
    //m_uart->print(arg);
    writeString(arg);  

  }
  
  //PGM_STRING_MAPPED_TO_RAM(EOL, "\r\n");
  //m_uart->print(EOL);
  writeString(AT_EOL);   
}


/****************************************************************
*FUNCTION NAME:simpleCommand
*FUNCTION     :AT commands with timeout 
*INPUT        :command : AT commands, arg : AT arguments, timeout  // check with bharath
*OUTPUT       :bool
****************************************************************/
bool HAL_HC05::simpleCommand(
  const char *command, const char *arg, unsigned long timeout)
{
  startOperation(timeout);
  writeCommand(command, arg);
  return readOperationResult();
}



/****************************************************************
*FUNCTION NAME:readAddressWithCommand
*FUNCTION     :readAddressWithCommand
*INPUT        :*address : Bluetooth Address,&address :,timeout: timeout   // check with bharath
*OUTPUT       :boolean readOperationResult
****************************************************************/
bool HAL_HC05::readAddressWithCommand(BluetoothAddress &address, 
                                      const char *command_name, unsigned long timeout)
{
  startOperation(timeout);
  memset(address, 0, sizeof(BluetoothAddress));

  if (!command_name)
    return false;

  writeCommand(command_name, "?");

  /* Response should look like "+<command_name>:<NAP>:<UAP>:<LAP>",
   * where actual address will look like "1234:56:abcdef".
   */
  char response[HC05_ADDRESS_BUFSIZE + 20];
  char response_pattern[20];
  
  for(uint8_t i=0;i<sizeof(response);i++)
  {
    response[i] = '\0';
  }
  
  for(uint8_t i=0;i<sizeof(response_pattern);i++)
  {
    response_pattern[i] = '\0';
  }

  //PGM_STRING_MAPPED_TO_RAM(format, "+%s:");
  snprintf(response_pattern, sizeof(response_pattern), "+%s:", command_name);

  char *address_part = readResponseWithPrefix(response, sizeof(response), response_pattern);

  if (m_errCode != HC05_OK)
    return false;

  if (!address_part)
    return readOperationResult() && false;

  if (!parseBluetoothAddress(address, address_part, ':'))
    return readOperationResult() && false;

  return readOperationResult();
}


/****************************************************************
*FUNCTION NAME:writeAddressWithCommand
*FUNCTION     :writeAddressWithCommand
*INPUT        :const &address:BluetoothAddress,command_name,timeout : timeout       // check with bharath
*OUTPUT       :boolean 
****************************************************************/
bool HAL_HC05::writeAddressWithCommand(const BluetoothAddress &address,
  const char *command_name, unsigned long timeout)
{
  char command[20];
  
  //PGM_STRING_MAPPED_TO_RAM(format, "%s=");
  snprintf(command, sizeof(command), "%s=", command_name);

  char address_str[HC05_ADDRESS_BUFSIZE];
  printBluetoothAddress(address_str, address, ',');

  return simpleCommand(command, address_str, timeout);
}
/****************************************************************
*FUNCTION NAME:parseBluetoothAddress
*FUNCTION     :parseBluetoothAddress
*INPUT        :const &address:BluetoothAddress,command_name,timeout : timeout       
*OUTPUT       :boolean 
****************************************************************/
bool HAL_HC05::parseBluetoothAddress(
  BluetoothAddress &address, const char *address_str, char delimiter)
{
  /* Address should look like "+ADDR:<NAP>:<UAP>:<LAP>",
   * where actual address will look like "1234:56:abcdef".
   */
  if (!address || !address_str)
    return false;

  char *digits_ptr = const_cast<char*>(address_str);
  uint8_t NAP[2];
  *((uint16_t*)NAP) = htoul(digits_ptr);
  digits_ptr = strchrnul(digits_ptr, delimiter);

  if (*digits_ptr != delimiter)
    return false;

  uint8_t UAP = htoul(++digits_ptr);
  digits_ptr = strchrnul(digits_ptr, delimiter);

  if (*digits_ptr != delimiter)
    return false;

  uint8_t LAP[4];
  *((uint32_t*)LAP) = htoul(++digits_ptr);

  address[0] = NAP[1];
  address[1] = NAP[0];
  address[2] = UAP;
  address[3] = LAP[2];
  address[4] = LAP[1];
  address[5] = LAP[0];

  return true;
}

/****************************************************************
*FUNCTION NAME:printBluetoothAddress
*FUNCTION     :printBluetoothAddress
*INPUT        :const &address:BluetoothAddress,command_name,timeout : timeout       
*OUTPUT       :boolean 
****************************************************************/
int HAL_HC05::printBluetoothAddress(char *address_str,
  const BluetoothAddress &address, char delimiter)
{
  if (!address || !address_str)
    return 0;

  uint8_t NAP[2];
  NAP[0] = address[1];
  NAP[1] = address[0];

  uint8_t UAP = address[2];

  uint8_t LAP[4];
  LAP[0] = address[5];
  LAP[1] = address[4];
  LAP[2] = address[3];
  LAP[3] = 0;

  //PGM_STRING_MAPPED_TO_RAM(format, "%x%c%x%c%lx");

  int written = snprintf(address_str, HC05_ADDRESS_BUFSIZE, "%x%c%x%c%lx",
    *reinterpret_cast<const uint16_t*>(NAP),
    delimiter, UAP, delimiter,
    *reinterpret_cast<const uint32_t*>(LAP));

  return written;
}
/****************************************************************
*FUNCTION NAME:readResponseWithPrefix
*FUNCTION     :readResponseWithPrefix
*INPUT        :buffer,buffer_size,prefix;
*OUTPUT       :none
****************************************************************/
char *HAL_HC05::readResponseWithPrefix(
  char *buffer, size_t buffer_size, const char *prefix)
{
  if (!buffer || buffer_size <= 1)
    return 0;

  uint16_t response_length = readLine(buffer, buffer_size);
  char *postfix = skipPrefix(buffer, response_length, prefix);

  if (!postfix)
    *buffer = 0;

  return postfix;
}

/****************************************************************
*FUNCTION NAME:skipPrefix
*FUNCTION     :Skips 
*INPUT        :str , str_length , prefix ; //Check wtih bharath
*OUTPUT       :char (but returnvalu s integer)
****************************************************************/
char *HAL_HC05::skipPrefix(char *str, uint16_t str_length, const char *prefix)
{
  if (!str || str_length == 0 || !prefix)
    return 0;

  uint16_t prefix_length = strlen(prefix);

  if (str_length >= prefix_length && strncmp(str, prefix, prefix_length) == 0)
    return str + prefix_length;

  return 0;
}


/****************************************************************
*FUNCTION NAME:getLastError
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
// HC05_Result HAL_HC05::getLastError() const 
// {
//   return m_errorCode;
// }

/****************************************************************
*FUNCTION NAME:readOperationResult
*FUNCTION     :recieve data // check with bharath
*INPUT        :none
*OUTPUT       :none
****************************************************************/
bool HAL_HC05::readOperationResult()
{
  char response[15];
  readLine(response, sizeof(response));

  //PGM_STRING_MAPPED_TO_RAM(OK, "OK");
  return strcmp(response, "OK") == 0;
}


/****************************************************************
*FUNCTION NAME:getLine     // move to the top
*FUNCTION     :getLine
*INPUT        :buffer,buffer_size; //refer defines in header file
*OUTPUT       :uint16_t
****************************************************************/
uint16_t HAL_HC05::readLine(char *buffer, size_t buffer_size)
{
  uint8_t index = 0;
  if (!buffer || buffer_size <= 1)
    return 0;

  char *p = buffer;
  *p = 0;
  
  while(index < buffer_size)
  {
      if (isOperationTimedOut())
        goto EXIT_LOOP;
      else
      {
        *p = getc();
        p++;
        index++;    
        
        if(p[-1] == '\n' )
        {
          
          p -= 1;       // setting '/n' as null char
          *p = 0;
          
          p -= 1;       // setting '/r' as null char
          *p = 0;
          
           goto EXIT_LOOP;
        }
        
      }
    


  }
    EXIT_LOOP:

  //PGM_STRING_MAPPED_TO_RAM(error_prefix, "ERROR:(");

  if (char *error_code_str = skipPrefix(buffer, buffer_size, "ERROR:("))
    m_errCode = static_cast<HC05_Result>(htoul(error_code_str));
  else if (strcmp(buffer, "FAIL") == 0)
    m_errCode = HC05_FAIL;

  uint16_t num_bytes = p - buffer;
  return num_bytes;
}


#endif