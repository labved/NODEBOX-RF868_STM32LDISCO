#include "stm32l1xx.h"
#include "Settings.h"

#ifdef HAL_LORA_H

#if (_USE_LORA == 1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_E70-868T30S.h"

#include "stdio.h"
#include "string.h"
#include "stdarg.h"

Lora_var loraStruct;
char temp10[16];

/****************************************************************
*FUNCTION NAME:gpioInit
*FUNCTION     :Init
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::gpioInit(void)                             
{
  // Setting GPIO for M0
  RCC_AHBPeriphClockCmd(LORA_RCC_M0, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_M0_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_M0_InitStruct;

  LORA_GPIO_M0_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_M0_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_M0_InitStruct.GPIO_Pin = LORA_PIN_M0;
  LORA_GPIO_M0_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_M0_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_M0, &LORA_GPIO_M0_InitStruct);

  // Setting GPIO for M1
  RCC_AHBPeriphClockCmd(LORA_RCC_M1, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_M1_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_M1_InitStruct;

  LORA_GPIO_M1_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_M1_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_M1_InitStruct.GPIO_Pin = LORA_PIN_M1;
  LORA_GPIO_M1_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_M1_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_M1, &LORA_GPIO_M1_InitStruct);

  // Setting GPIO for M2
  RCC_AHBPeriphClockCmd(LORA_RCC_M2, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_M2_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_M2_InitStruct;

  LORA_GPIO_M2_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_M2_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_M2_InitStruct.GPIO_Pin = LORA_PIN_M2;
  LORA_GPIO_M2_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_M2_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_M2, &LORA_GPIO_M2_InitStruct);

  // Setting GPIO for LNAEN
  RCC_AHBPeriphClockCmd(LORA_RCC_LNAEN, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_LNAEN_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_LNAEN_InitStruct;

  LORA_GPIO_LNAEN_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_LNAEN_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_LNAEN_InitStruct.GPIO_Pin = LORA_PIN_LNAEN;
  LORA_GPIO_LNAEN_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_LNAEN_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_LNAEN, &LORA_GPIO_LNAEN_InitStruct);

    // Setting GPIO for PAEN
  RCC_AHBPeriphClockCmd(LORA_RCC_PAEN, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_PAEN_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_PAEN_InitStruct;

  LORA_GPIO_PAEN_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_PAEN_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_PAEN_InitStruct.GPIO_Pin = LORA_PIN_PAEN;
  LORA_GPIO_PAEN_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_PAEN_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_PAEN, &LORA_GPIO_PAEN_InitStruct);

  // Setting GPIO for RST
  RCC_AHBPeriphClockCmd(LORA_RCC_RST, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_RST_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_RST_InitStruct;

  LORA_GPIO_RST_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  LORA_GPIO_RST_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_RST_InitStruct.GPIO_Pin = LORA_PIN_RST;
  LORA_GPIO_RST_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_RST_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_RST, &LORA_GPIO_RST_InitStruct);

  // Setting GPIO for AUX
  RCC_AHBPeriphClockCmd(LORA_RCC_AUX, ENABLE); // Enable GPIO clock 
  RCC_APB2PeriphClockCmd(LORA_AUX_SYSCONFIG_APB, ENABLE); // Enable SYSCFG clock 
  
  GPIO_InitTypeDef LORA_GPIO_AUX_InitStruct;

  LORA_GPIO_AUX_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  LORA_GPIO_AUX_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_AUX_InitStruct.GPIO_Pin = LORA_PIN_AUX;
  LORA_GPIO_AUX_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LORA_GPIO_AUX_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;

  GPIO_Init(LORA_GPIO_AUX, &LORA_GPIO_AUX_InitStruct);
}
/****************************************************************
*FUNCTION NAME:uartInit
*FUNCTION     :Init
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::uartInit(void)                             
{
  RCC_AHBPeriphClockCmd(LORA_RCC_TX, ENABLE); // Enable GPIO clock  
  RCC_APB1PeriphClockCmd(LORA_RCC_APB, ENABLE); // Enable USART clock

  // GPIO Setting for TX
  GPIO_InitTypeDef   LORA_GPIO_InitStruct;

  LORA_GPIO_InitStruct.GPIO_Pin = LORA_PIN_TX;
  LORA_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  LORA_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  LORA_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  
  GPIO_Init(LORA_GPIO_TX, &LORA_GPIO_InitStruct);
  
  // GPIO Setting for RX
  RCC_AHBPeriphClockCmd(LORA_RCC_RX, ENABLE); // Enable GPIO clock  

  LORA_GPIO_InitStruct.GPIO_Pin = LORA_PIN_RX;
  LORA_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  LORA_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  LORA_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  LORA_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  
  GPIO_Init(LORA_GPIO_RX, &LORA_GPIO_InitStruct);
  
  GPIO_PinAFConfig(LORA_GPIO_TX, LORA_AF_PIN_TX, LORA_GPIO_AF);
  GPIO_PinAFConfig(LORA_GPIO_RX, LORA_AF_PIN_RX, LORA_GPIO_AF);
  
  
  // UART Setting
  USART_DeInit(LORA_USART_CH);
  
  USART_InitTypeDef  LORA_USART_InitStruct;        

  LORA_USART_InitStruct.USART_BaudRate = LORA_BAUDRATE;
  LORA_USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  LORA_USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  LORA_USART_InitStruct.USART_Parity = USART_Parity_No;
  LORA_USART_InitStruct.USART_StopBits = USART_StopBits_1;
  LORA_USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  
  USART_Init(LORA_USART_CH, &LORA_USART_InitStruct);
  
  USART_Cmd(LORA_USART_CH, ENABLE);
  
  NVIC_InitTypeDef   NVIC_LORA_InitStruct;

  NVIC_LORA_InitStruct.NVIC_IRQChannel = LORA_USART_CH_IRQn;
  NVIC_LORA_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_LORA_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_LORA_InitStruct.NVIC_IRQChannelSubPriority = 0;

  NVIC_Init(&NVIC_LORA_InitStruct);
  
// NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
//NVIC_EnableIRQ(USART2_IRQn);
  
  USART_ITConfig(LORA_USART_CH, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(LORA_USART_CH, ENABLE);
    
}
/****************************************************************
*FUNCTION NAME:hardReset
*FUNCTION     :Reset Pin
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::hardReset(void)
{
  
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::set2Tx(void)
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
void HAL_E70_868T30S::set2Rx(void)
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
void HAL_E70_868T30S::req2Tx(void)
{
  putc(XON_BYTE);
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::req2Rx(void)
{
  putc(XOFF_BYTE);
}
/****************************************************************
*FUNCTION NAME:set2Tx
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::setTxMode(void)
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
void HAL_E70_868T30S::setRxMode(void)
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
void HAL_E70_868T30S::flowControl(void)
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
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
void  HAL_E70_868T30S::putc(byte ch)                        
{
  while(!(USART_GetFlagStatus(LORA_USART_CH, USART_FLAG_TXE)));

  USART_SendData(LORA_USART_CH, ch);

  USART_ClearFlag(LORA_USART_CH, USART_FLAG_TC); 
  
}
/****************************************************************
*FUNCTION NAME:getc
*FUNCTION     :send data in UART
*INPUT        :data : data to send
*OUTPUT       :none
****************************************************************/
byte  HAL_E70_868T30S::getc(void)                            
{
  byte ch;
  
  while (!(USART_GetFlagStatus(LORA_USART_CH, USART_FLAG_RXNE)));

  ch = USART_ReceiveData(LORA_USART_CH);
  
  //USART_ClearFlag(LORA_USART_CH, USART_FLAG_RXNE);
       
  return ch;
  
}
/****************************************************************
*FUNCTION NAME:HAL_UART_Transmit
*FUNCTION     :HAL_UART_Transmit
*INPUT        :USARTx,str,size, timeout;
*OUTPUT       :none
****************************************************************/
void HAL_E70_868T30S::HAL_UART_Transmit(USART_TypeDef* USARTx,uint8_t *str,
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
void  HAL_E70_868T30S::write8(uint8_t *data) // ASK BHARAT FOR CONFIRMATION
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
    delay_ms(10);
  #endif
}
/****************************************************************
*FUNCTION NAME:writeRaw
*FUNCTION     :writeRaw
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
void  HAL_E70_868T30S::writeRaw(uint8_t *data, uint16_t len)
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
    delay_ms(10);
  #endif
}
/****************************************************************
*FUNCTION NAME:writeString
*FUNCTION     :writeString
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
void	HAL_E70_868T30S::writeString(char *str)
{
  USART_ITConfig(LORA_USART_CH, USART_IT_RXNE, ENABLE);	

  #if (_SIM80X_DMA_TRANSMIT==1)
  while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
    delay_ms(10);
    HAL_UART_Transmit_DMA(&_SIM80X_USART,(uint8_t*)str,strlen(str));
  while(_SIM80X_USART.hdmatx->State != HAL_DMA_STATE_READY)
    delay_ms(10);
  #else
    HAL_UART_Transmit(LORA_USART_CH,(uint8_t*)str,strlen(str),100);
    delay_ms(10);
  #endif
}
/****************************************************************
*FUNCTION NAME:coreSendAtCommand
*FUNCTION     :coreSendAtCommand
*INPUT        :unsigned i, char *a, unsigned r
*OUTPUT       :byte
****************************************************************/
uint8_t  HAL_E70_868T30S::sendAtCommand(char const *AtCommand,int32_t  MaxWaiting_ms,uint8_t const HowMuchAnswers,...)
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
*FUNCTION NAME:readLine
*FUNCTION     :readLine
*INPUT        :message, buf_size
*OUTPUT       :uint16_t
****************************************************************/
uint16_t HAL_E70_868T30S::readLine(char *buffer, size_t buffer_size)   
{

  uint16_t index = 0;
  if (!buffer || buffer_size <= 1)
    return 0;

  memset(temp10, 0, sizeof(temp10));
  char *p = buffer;
  *p = 0;
  
  while(index < buffer_size)
  {
     
    *p = getc();
     loraStruct.UsartRxBuffer[index] = *p;

    // if(index == 2)
    //   *p = 0;

    if(*p == '\n')
    {
      temp10[0] = p[-5];
      temp10[1] = p[-4];
      temp10[2] = p[-3];
      temp10[3] = p[-2];
      temp10[4] = p[-1];
      temp10[5] = p[0];
      
      uint16_t val = strncmp(temp10, "\r\nOK\r\n", sizeof("\r\nOK\r\n"));
      
      if(val == 0 ) 
        goto EXIT_LOOP;

    }
    
    p++;
    index++;

  }
  EXIT_LOOP:
  return index;
}

/****************************************************************
*FUNCTION NAME:setRssiMode
*FUNCTION     :setRssiMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setRssiMode(void)
{
  GPIO_ResetBits(LORA_GPIO_M0, LORA_PIN_M0);  //need to be pull down not floated
  GPIO_ResetBits(LORA_GPIO_M1, LORA_PIN_M1);  //need to be pull down not floated
  GPIO_ResetBits(LORA_GPIO_M2, LORA_PIN_M2);  //need to be pull down not floated

}

/****************************************************************
*FUNCTION NAME:setContinuousMode
*FUNCTION     :setContinuousMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setContinuousMode(void)
{
  GPIO_SetBits(LORA_GPIO_M0, LORA_PIN_M0);  
  GPIO_ResetBits(LORA_GPIO_M1, LORA_PIN_M1);  //need to be pull down not floated
  GPIO_ResetBits(LORA_GPIO_M2, LORA_PIN_M2);  //need to be pull down not floated
  
}

/****************************************************************
*FUNCTION NAME:setSubPackageMode
*FUNCTION     :setSubPackageMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setSubPackageMode(void)
{
  GPIO_ResetBits(LORA_GPIO_M0, LORA_PIN_M0);  //need to be pull down not floated
  GPIO_SetBits(LORA_GPIO_M1, LORA_PIN_M1);
  GPIO_ResetBits(LORA_GPIO_M2, LORA_PIN_M2);  //need to be pull down not floated
  
}

/****************************************************************
*FUNCTION NAME:setWakeUpMode
*FUNCTION     :setWakeUpMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setWakeUpMode(void)
{
  GPIO_ResetBits(LORA_GPIO_M0, LORA_PIN_M0);  //need to be pull down not floated
  GPIO_ResetBits(LORA_GPIO_M1, LORA_PIN_M1);  //need to be pull down not floated
  GPIO_SetBits(LORA_GPIO_M2, LORA_PIN_M2);  
  
}

/****************************************************************
*FUNCTION NAME:setConfigurationMode
*FUNCTION     :setConfigurationMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setConfigurationMode(void)
{
  GPIO_SetBits(LORA_GPIO_M0, LORA_PIN_M0);
  GPIO_ResetBits(LORA_GPIO_M1, LORA_PIN_M1);  //need to be pull down not floated
  GPIO_SetBits(LORA_GPIO_M2, LORA_PIN_M2);
  
}

/****************************************************************
*FUNCTION NAME:setPowerSavingMode
*FUNCTION     :setPowerSavingMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setPowerSavingMode(void)
{
  GPIO_ResetBits(LORA_GPIO_M0, LORA_PIN_M0);  //need to be pull down not floated
  GPIO_SetBits(LORA_GPIO_M1, LORA_PIN_M1);  
  GPIO_SetBits(LORA_GPIO_M2, LORA_PIN_M2);
  
}

/****************************************************************
*FUNCTION NAME:setSleepMode
*FUNCTION     :setSleepMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setSleepMode(void)
{
  GPIO_SetBits(LORA_GPIO_M0, LORA_PIN_M0);
  GPIO_SetBits(LORA_GPIO_M1, LORA_PIN_M1);
  GPIO_SetBits(LORA_GPIO_M2, LORA_PIN_M2);
  
}

/****************************************************************
*FUNCTION NAME:setRfTx
*FUNCTION     :setRfTx
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setRfTx(void)
{
  GPIO_ResetBits(LORA_GPIO_LNAEN, LORA_PIN_LNAEN);
  GPIO_SetBits(LORA_GPIO_PAEN, LORA_PIN_PAEN);
  
}

/****************************************************************
*FUNCTION NAME:setRFRx
*FUNCTION     :setRFRx
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::setRfRx(void)
{
  GPIO_SetBits(LORA_GPIO_LNAEN, LORA_PIN_LNAEN);
  GPIO_ResetBits(LORA_GPIO_PAEN, LORA_PIN_PAEN);
  
}

/****************************************************************
*FUNCTION NAME:resetTxRx
*FUNCTION     :resetTxRx
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	HAL_E70_868T30S::resetRfTxRx(void)
{
  GPIO_ResetBits(LORA_GPIO_LNAEN, LORA_PIN_LNAEN); 
  GPIO_ResetBits(LORA_GPIO_PAEN, LORA_PIN_PAEN);  
}



#endif

#endif