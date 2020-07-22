#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_LORA
#define _USE_LORA
#endif

#if (_USE_LORA == 1)


class LIB_E70_868T30S    
{
public:
  void Init(void);
  void PoweronReset(void);
  void RegConfigSettings(void);
  void setAtMode(void);
  void setDataMode(void);
  bool softReset(unsigned long timeout );  

public:
  //HardwareSerial *m_uart;
  unsigned long m_timeout;
  unsigned long m_ticksAtStart;

  // AT COMMANDS METHODS
  void testCommand(void);

    
};


#endif