#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_MODEL
  #define _USE_MODEL
#endif

#if(_USE_MODEL==1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

#include "intrinsics.h"

#include "HAL_MODEL.h"

extern Sim80x_t               Sim80x;


/****************************************************************
*FUNCTION NAME:gpioInit
*FUNCTION     :Initialise GPIO Ports
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::gpioInit(void)                             
{

 
}

/****************************************************************
*FUNCTION NAME:uartInit
*FUNCTION     :Initialise UART ports
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::uartInit(void)                             
{
      
}

/****************************************************************
*FUNCTION NAME:hardReset
*FUNCTION     :Reset Pin
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::hardReset(void)
{
  
}

/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :set to send data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::set2Tx(void)
{
  req2Rx();
  setTxMode();
}

/****************************************************************
*FUNCTION NAME:set2Rx
*FUNCTION     :set to receive data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::set2Rx(void)
{
  req2Tx();
  setRxMode();
}

/****************************************************************
*FUNCTION NAME:req2Tx
*FUNCTION     :request to send data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::req2Tx(void)
{
  putc(XON_BYTE);
}

/****************************************************************
*FUNCTION NAME:req2Rx
*FUNCTION     :request to recieve data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::req2Rx(void)
{
  putc(XOFF_BYTE);
}

/****************************************************************
*FUNCTION NAME:setTxMode
*FUNCTION     :set flag to send data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::setTxMode(void)
{
  //CP2102TxFlag = 1;
  //CP2102RxFlag = 0;
}

/****************************************************************
*FUNCTION NAME:setRxMode
*FUNCTION     :set flag to recieve data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::setRxMode(void)
{
  //CP2102TxFlag = 0;
  //CP2102RxFlag = 1 ; 
}

/****************************************************************
*FUNCTION NAME:flowControl
*FUNCTION     :send data in UART
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::flowControl(void)
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
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void  HAL_MODEL::putc(byte ch)                        
{
  while(!(USART_GetFlagStatus(GSM_USART_CH, USART_FLAG_TXE)));

  USART_SendData(GSM_USART_CH, ch);  
}

/****************************************************************
*FUNCTION NAME:getc
*FUNCTION     :recieve data from UART
*INPUT        :none
*OUTPUT       :byte
****************************************************************/
byte  HAL_MODEL::getc(void)                            
{
  byte ch;
  
  while (!(USART_GetFlagStatus(GSM_USART_CH, USART_FLAG_RXNE)));

  ch = USART_ReceiveData(GSM_USART_CH);
         
  return ch;
  
}

/****************************************************************
*FUNCTION NAME:HAL_UART_Transmit
*FUNCTION     :HAL_UART_Transmit
*INPUT        :str,size, timeout;
*OUTPUT       :none
****************************************************************/
void HAL_MODEL::HAL_UART_Transmit(uint8_t *str, uint8_t size,uint8_t timeout)
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
*INPUT        :data
*OUTPUT       :none
****************************************************************/
void  HAL_MODEL::write8(uint8_t *data) // ASK BHARAT FOR CONFIRMATION
{
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,Data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(data, 1,100);
    //delay_ms(10);
  #endif
}

/****************************************************************
*FUNCTION NAME:writeRaw
*FUNCTION     :writeRaw
*INPUT        :Data,len
*OUTPUT       :none
****************************************************************/
void  HAL_MODEL::writeRaw(uint8_t *Data,uint16_t len)
{
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
      HAL_UART_Transmit_DMA(&_SIM80X_USART,Data,len);
    while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
      delay_ms(10);
  #else
    HAL_UART_Transmit(Data,len,100);
    //delay_ms(10);
  #endif

}

/****************************************************************
*FUNCTION NAME:writeString
*FUNCTION     :writeString
*INPUT        :str
*OUTPUT       :none
****************************************************************/
void	HAL_MODEL::writeString(char *str)
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
*FUNCTION     :sendAtCommand
*INPUT        :AtCommand,MaxWaiting_ms,HowMuchAnswers,...
*OUTPUT       :byte
****************************************************************/
uint8_t  HAL_MODEL::sendAtCommand(char const *AtCommand,int32_t  MaxWaiting_ms,uint8_t const HowMuchAnswers,...)
{
  // while(Sim80x.Status.Busy == 1)
  // {
  //   delay_ms(100);
  // }

  Sim80x.Status.Busy = 1;
  // Sim80x.AtCommand.FindAnswer = 0;
  // Sim80x.AtCommand.ReceiveAnswerExeTime=0;
  // Sim80x.AtCommand.SendCommandStartTime = HAL_GetTick();
  // Sim80x.AtCommand.ReceiveAnswerMaxWaiting = MaxWaiting_ms;
  
  memset(Sim80x.AtCommand.ReceiveAnswer,0,sizeof(Sim80x.AtCommand.ReceiveAnswer));  // clear buffer
  
  // dynamic allocation of arguments
  va_list tag;
  va_start (tag,HowMuchAnswers);
  
  char *arg[10];

  for(uint8_t i=0; i<HowMuchAnswers ; i++)
  {
    arg[i] = va_arg (tag, char *);	
    strncpy(Sim80x.AtCommand.ReceiveAnswer[i],arg[i],sizeof(Sim80x.AtCommand.ReceiveAnswer[0]));
  }

  va_end (tag);	

  // sending AT COMMANDS
  strncpy(Sim80x.AtCommand.SendCommand,AtCommand,sizeof(Sim80x.AtCommand.SendCommand)); 
  writeString(Sim80x.AtCommand.SendCommand); 
  
  //readLine(Sim80x.UsartRxBuffer, sizeof(Sim80x.UsartRxBuffer));
  
  // while(MaxWaiting_ms > 0)
  // {
  //   delay_ms(10);
  //   if(Sim80x.AtCommand.FindAnswer > 0)
  //     return Sim80x.AtCommand.FindAnswer;    
  //   MaxWaiting_ms-=10;
  // }

  memset(Sim80x.AtCommand.ReceiveAnswer,0,sizeof(Sim80x.AtCommand.ReceiveAnswer));
  
  Sim80x.Status.Busy=0;

  return Sim80x.AtCommand.FindAnswer;
}




#endif
