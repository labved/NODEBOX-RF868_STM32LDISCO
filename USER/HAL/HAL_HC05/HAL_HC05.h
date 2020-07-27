#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "string.h"

#ifndef HAL_HC05_H
  #define HAL_HC05_H

#if(_USE_BT==1)

typedef uint8_t BluetoothAddress[6];

class HAL_HC05
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
    void                    uartInit(uint16_t baudrate);
    void                    hardReset(void);
    void                    write8(uint8_t *data);
    void                    writeRaw(uint8_t *data, uint16_t size);
    void	                  writeString(char const *data);
    uint8_t                 sendAtCommand(char const *atcommand,int32_t  maxwaiting_ms,uint8_t const howmuchanswers,...);
    uint16_t                readLine(char *buffer, size_t buffer_size); 


  public:
    // original HC05

    void writeCommand(const char *command, const char *arg = 0);//checked  //DELETE TH COMMAND
    bool simpleCommand(const char *command_name, const char *arg, unsigned long timeout);//checked //DELETE THE COMMAND
    bool readOperationResult(void);
    bool readAddressWithCommand(BluetoothAddress &address, const char *command_name, 
                                unsigned long timeout); //checked //need to check
    bool writeAddressWithCommand(const BluetoothAddress &address, const char *command_name,
                                 unsigned long timeout);        //checked //need to check
    static bool parseBluetoothAddress(BluetoothAddress &address, const char *address_str,
                                  char delimiter);      //checked  //need to check
    static int printBluetoothAddress(char *address_str, const BluetoothAddress &address,
                                 char delimiter);  //need to check
    char *readResponseWithPrefix(char *buffer, size_t buffer_size, const char *prefix); //checked
    static char *skipPrefix(char *str, uint16_t str_length, const char *prefix);        //checked    

    //HC05_Result getLastError() const;
 

      
};



#endif

extern HAL_HC05 hc05;

#endif