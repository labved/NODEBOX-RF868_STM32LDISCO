#include "stm32l1xx.h"
#include "Settings.h"

#ifndef HAL_LORA_H
#define HAL_LORA_H

#if (_USE_LORA == 1)

#include "Shortcodes.h"

class HAL_E70_868T30S
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
    
    void                    HAL_UART_Transmit(USART_TypeDef* USARTx,uint8_t *str,uint8_t size,uint8_t timeout);
    void                    write8(uint8_t *data);
    void                    writeRaw(uint8_t *data, uint16_t len);
    void	                writeString(char *str);
    uint8_t                 sendAtCommand(char const *AtCommand,int32_t  MaxWaiting_ms,uint8_t const HowMuchAnswers,...);

    uint16_t                readLine(char *buffer, size_t buffer_size); 
    
    void                    setRssiMode(void);
    void                    setContinuousMode(void);
    void                    setSubPackageMode(void);
    void                    setWakeUpMode(void);
    void                    setConfigurationMode(void);
    void                    setPowerSavingMode(void);
    void                    setSleepMode(void);
    void                    setRfTx(void);    //need data 
    void                    setRfRx(void);    //need data
    void                    resetRfTxRx(void);    //need data

};

extern  HAL_E70_868T30S e70_868t30s;

#endif
#endif