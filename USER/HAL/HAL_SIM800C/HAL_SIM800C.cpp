#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_SIM800C.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "intrinsics.h"

#ifdef HAL_SIM800C_H

#if(_USE_GSM==1)



extern Sim80x_t               Sim80x;

/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :set UART to Tx mode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::set2Tx(void)
{
  req2Rx();
  setTxMode();
}

/****************************************************************
*FUNCTION NAME:set2Rx
*FUNCTION     :set UART to Rx mode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::set2Rx(void)
{
  req2Tx();
  setRxMode();
}

/****************************************************************
*FUNCTION NAME:req2Tx
*FUNCTION     : req2Tx
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::req2Tx(void)
{
  putc(XON_BYTE);
}

/****************************************************************
*FUNCTION NAME:req2Rx
*FUNCTION     :req2Rx
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::req2Rx(void)
{
  putc(XOFF_BYTE);
}

/****************************************************************
*FUNCTION NAME:setTxMode
*FUNCTION     :set Tx flags
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::setTxMode(void)
{
  //CP2102TxFlag = 1;
  //CP2102RxFlag = 0;
}

/****************************************************************
*FUNCTION NAME:setRxMode
*FUNCTION     :set Rx flags
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::setRxMode(void)
{
  //CP2102TxFlag = 0;
  //CP2102RxFlag = 1 ; 
}

/****************************************************************
*FUNCTION NAME:flowControl
*FUNCTION     :flowControl
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::flowControl(void)
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
*FUNCTION NAME:putc
*FUNCTION     :send data in UART
*INPUT        :ch
*OUTPUT       :void
****************************************************************/
void  HAL_SIM800C::putc(byte ch)                        
{
  while(!(USART_GetFlagStatus(GSM_USART_CH, USART_FLAG_TXE)));

  USART_SendData(GSM_USART_CH, ch);
  
  while(!(USART_GetFlagStatus(GSM_USART_CH, USART_FLAG_TC)));
  
}

/****************************************************************
*FUNCTION NAME:getc
*FUNCTION     :recieve data in UART
*INPUT        :void
*OUTPUT       :byte
****************************************************************/
byte HAL_SIM800C::getc(void)                            
{ 
  while (!(USART_GetFlagStatus(GSM_USART_CH, USART_FLAG_RXNE)));
    
  return USART_ReceiveData(GSM_USART_CH); 
}

/****************************************************************
*FUNCTION NAME:HAL_UART_Transmit
*FUNCTION     :transmit block of data
*INPUT        :data, size, timeout
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::HAL_UART_Transmit(uint8_t *data, uint8_t size, uint8_t timeout)
{
  char temp;
  for(uint8_t i=0;i<size;i++)
  {
      temp = *data;
      putc(temp);
      data++;
  }

}

/****************************************************************
*FUNCTION NAME:gpioInit
*FUNCTION     :Initialize GPIO pins
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::gpioInit(void)                             
{
  // Setting GPIO for NET
  RCC_AHBPeriphClockCmd(GSM_RCC_NET, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(GSM_NET_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef GSM_GPIO_NET_InitStruct;

  GSM_GPIO_NET_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GSM_GPIO_NET_InitStruct.GPIO_OType = GPIO_OType_PP;
  GSM_GPIO_NET_InitStruct.GPIO_Pin = GSM_PIN_NET;
  GSM_GPIO_NET_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GSM_GPIO_NET_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(GSM_GPIO_NET, &GSM_GPIO_NET_InitStruct);
  
  // Setting GPIO for RST
  RCC_AHBPeriphClockCmd(GSM_RCC_RST, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(GSM_RST_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef GSM_GPIO_RST_InitStruct;

  GSM_GPIO_RST_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GSM_GPIO_RST_InitStruct.GPIO_OType = GPIO_OType_PP;
  GSM_GPIO_RST_InitStruct.GPIO_Pin = GSM_PIN_RST;
  GSM_GPIO_RST_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GSM_GPIO_RST_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(GSM_GPIO_RST, &GSM_GPIO_RST_InitStruct);
  
  GPIO_ResetBits(GSM_GPIO_RST, GSM_PIN_RST);      //  RST pulled to LOW
  delay_ms(300);   
 
  GPIO_SetBits(GSM_GPIO_RST, GSM_PIN_RST);       // RST floating to HIGH
  delay_ms(1000);
  

}

/****************************************************************
*FUNCTION NAME:uartInit
*FUNCTION     :Initialize USART pins 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::uartInit(void)                             
{
  
  RCC_AHBPeriphClockCmd(GSM_RCC_TX, ENABLE); // Enable GPIO clock  

  // GPIO Setting for TX
  GPIO_InitTypeDef   GSM_GPIO_InitStruct;

  GSM_GPIO_InitStruct.GPIO_Pin = GSM_PIN_TX;
  GSM_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GSM_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GSM_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GSM_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  
  GPIO_Init(GSM_GPIO_TX, &GSM_GPIO_InitStruct);
  
  // GPIO Setting for RX
  RCC_AHBPeriphClockCmd(GSM_RCC_RX, ENABLE); // Enable GPIO clock  

  GSM_GPIO_InitStruct.GPIO_Pin = GSM_PIN_RX;
  GSM_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GSM_GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GSM_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GSM_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  
  GPIO_Init(GSM_GPIO_RX, &GSM_GPIO_InitStruct);
  
  GPIO_PinAFConfig(GSM_GPIO_TX, GSM_AF_PIN_TX, GSM_GPIO_AF);
  GPIO_PinAFConfig(GSM_GPIO_RX, GSM_AF_PIN_RX, GSM_GPIO_AF);
  
  // UART Setting
  USART_DeInit(GSM_USART_CH);

  RCC_APB1PeriphClockCmd(GSM_RCC_APB, ENABLE); // Enable USART clock
  
  USART_InitTypeDef  GSM_USART_InitStruct;        

  GSM_USART_InitStruct.USART_BaudRate = GSM_BAUDRATE;
  GSM_USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  GSM_USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  GSM_USART_InitStruct.USART_Parity = USART_Parity_No;
  GSM_USART_InitStruct.USART_StopBits = USART_StopBits_1;
  GSM_USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  
  USART_Init(GSM_USART_CH, &GSM_USART_InitStruct);
  
  USART_Cmd(GSM_USART_CH, ENABLE);
  
  // INTERRUPT SETUP
  /*
  NVIC_InitTypeDef   NVIC_GSM_InitStruct;

  NVIC_GSM_InitStruct.NVIC_IRQChannel = GSM_USART_CH_IRQn;
  NVIC_GSM_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_GSM_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_GSM_InitStruct.NVIC_IRQChannelSubPriority = 0;

  NVIC_Init(&NVIC_GSM_InitStruct);
  
  // NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  //NVIC_EnableIRQ(USART2_IRQn);
  
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);
  */  
    
}

/****************************************************************
*FUNCTION NAME:hardReset
*FUNCTION     :Reset via GPIO
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void HAL_SIM800C::hardReset(void)
{
  GPIO_ResetBits(GSM_GPIO_RST, GSM_PIN_RST);      //  RST pulled to LOW
  delay_ms(300);   
 
  GPIO_SetBits(GSM_GPIO_RST, GSM_PIN_RST);       // RST floating to HIGH
  delay_ms(1000);
}

/****************************************************************
*FUNCTION NAME:write8
*FUNCTION     :write only 1 byte of hex
*INPUT        :data
*OUTPUT       :void
****************************************************************/
void  HAL_SIM800C::write8(uint8_t *data) // ASK BHARAT FOR CONFIRMATION
{
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(data, 1,100);
    delay_ms(10);
  #endif
}

/****************************************************************
*FUNCTION NAME:writeRaw
*FUNCTION     :write multiple bytes in hex
*INPUT        :data, len
*OUTPUT       :void
****************************************************************/
void  HAL_SIM800C::writeRaw(uint8_t *data, uint16_t len)
{
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(data,len,100);
    delay_ms(10);
  #endif

}

/****************************************************************
*FUNCTION NAME:writeString
*FUNCTION     :Transmit bytes until null character
*INPUT        :str
*OUTPUT       :void
****************************************************************/
void	HAL_SIM800C::writeString(char const *str)
{
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
	  while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,(uint8_t*)str,strlen(str));
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit((uint8_t*)str,strlen(str),100);
    //delay_ms(10); 
  #endif
}

/****************************************************************
*FUNCTION NAME:sendAtCommand
*FUNCTION     :transmit and read response at commands in uart
*INPUT        :atcommand,wait_time, howmuchanswers,...
*OUTPUT       :index
****************************************************************/
uint8_t  HAL_SIM800C::sendAtCommand(char const *atcommand, int32_t  maxwaiting_ms, uint8_t const howmuchanswers,...)
{
  uint16_t index=0;
  
  while(Sim80x.Status.Busy == 1)
    delay_ms(100);

  Sim80x.Status.Busy = 1;
  Sim80x.AtCommand.FindAnswer = 0;
  Sim80x.AtCommand.ReceiveAnswerExeTime=0;
  //Sim80x.AtCommand.SendCommandStartTime = HAL_GetTick();
  Sim80x.AtCommand.ReceiveAnswerMaxWaiting = maxwaiting_ms;
  
  memset(Sim80x.AtCommand.ReceiveAnswer,0,sizeof(Sim80x.AtCommand.ReceiveAnswer)); // clear buffer
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));                     // clear buffer

  // va_list tag;
  // va_start (tag,howmuchanswers);
  
  // char *arg[10];
  // for(uint8_t i=0; i<howmuchanswers ; i++)
  // {
  //   arg[i] = va_arg (tag, char *);	
  //   strncpy(Sim80x.AtCommand.ReceiveAnswer[i],arg[i],sizeof(Sim80x.AtCommand.ReceiveAnswer[0]));
  // }
  // va_end (tag);	
  
  strncpy(Sim80x.AtCommand.SendCommand,atcommand,sizeof(Sim80x.AtCommand.SendCommand)); 
  writeString(Sim80x.AtCommand.SendCommand); 
  
  index = readLine(Sim80x.UsartRxBuffer, sizeof(Sim80x.UsartRxBuffer));
  
  if(index == 0)
      memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 
    
  // while(maxwaiting_ms > 0)
  // {
  //   delay_ms(10);
  //   if(Sim80x.AtCommand.FindAnswer > 0)
  //     return Sim80x.AtCommand.FindAnswer;    
  //   maxwaiting_ms-=10;
  // }

  delay_ms(maxwaiting_ms);

  Sim80x.Status.Busy=0;
  return Sim80x.AtCommand.FindAnswer;
}

/****************************************************************
*FUNCTION NAME:readLine
*FUNCTION     :read AT command response from uart
*INPUT        :message, buf_size
*OUTPUT       :uint16_t
****************************************************************/
uint16_t HAL_SIM800C::readLine(char *buffer, size_t buffer_size)   
{
  char temp1[10];
  uint16_t index = 0;
  uint16_t cnt = 0;
  char *temp2;
  uint8_t i, size;
    
  if (!buffer || buffer_size <= 1)
   return 0;

  char *p = buffer;
  
  while(index < buffer_size)
  {
    if (isOperationTimedOut())
      goto EXIT_LOOP;
    else
      *p = getc();
         
   if(*p == '\n')
   {
     cnt++;
     
     if(cnt%2 == 0)
     {
      size = sizeof(temp1)-1;
      for( i=0;i<=size;i++)
        temp1[size-i] = p[-i];

     i=0;
      
      while(temp1[i] =='\0')
        temp1[i++]+=1;
      
      temp2 = strstr(temp1, "\r\nOK\r\n");
      if(strncmp(temp2, "\r\nOK\r\n", strlen("\r\nOK\r\n")) == 0)
        goto EXIT_LOOP;
        
      temp2 = strstr(temp1, "\r\nERROR\r\n");
      if(strncmp( temp2, "\r\nERROR\r\n", strlen("\r\nERROR\r\n")) == 0)
        return 0;
     }
   }
    
    p++;
    index++;
  }

  EXIT_LOOP:
  return index;
}

#endif
#endif
