#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef HAL_SIM800C_H
  #define HAL_SIM800C_H

#if(_USE_GSM==1)

#include "Shortcodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class HAL_SIM800C   
{
  private:
    void                    set2Tx(void);
    void                    set2Rx(void);
    void                    req2Tx(void);
    void                    req2Rx(void);
    void                    setTxMode(void);
    void                    setRxMode(void);
    void                    flowControl(void);  
    void                    putc(byte ch);                
    char                    getc(void);
    void                    HAL_UART_Transmit(uint8_t *data, uint8_t size, uint8_t timeout);
         
  public:
    void                    gpioInit(void);
    void                    uartInit(void);
    void                    hardReset(void);
    void                    write8(uint8_t *data);
    void                    writeRaw(uint8_t *data, uint16_t size);
    void	                  writeString(char const *data);
    uint8_t                 sendAtCommand(char const *atcommand,int32_t  maxwaiting_ms,uint8_t const howmuchanswers,...);
    uint16_t                readLine(char *buffer, size_t buffer_size); 

    //void HAL_UART_Receive_IT(USART_TypeDef* USARTx,uint8_t *UsartRxTemp,bool state);
    
  
};

extern HAL_SIM800C sim800c;

//void GSM_IRQHandler(void);


#endif


#endif
