#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_MODEL
  #define _USE_MODEL
#endif

#if(_USE_MODEL==1)

#include "Shortcodes.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class HAL_MODEL   
{
  public:
    void                    gpioInit(void);
    void                    uartInit(void);
    void                    hardReset(void);
    void                    set2Tx(void);
    void                    set2Rx(void);
    void                    req2Tx(void);
    void                    req2Rx(void);
    void                    setTxMode(void);
    void                    setRxMode(void);
    void                    flowControl(void);  
    void                    putc(byte ch);                
    char                    getc(void);
    
    void                    HAL_UART_Transmit(uint8_t *str,uint8_t size,uint8_t timeout);
    void                    write8(uint8_t *data);
    void                    writeRaw(uint8_t *data, uint16_t len);
    void	                writeString(char *str);
    uint8_t                 sendAtCommand(char const *AtCommand,int32_t  MaxWaiting_ms,uint8_t const HowMuchAnswers,...);

      
    public:

    //void HAL_UART_Receive_IT(USART_TypeDef* USARTx,uint8_t *UsartRxTemp,bool state);
    
  
};

extern HAL_MODEL model;

#endif