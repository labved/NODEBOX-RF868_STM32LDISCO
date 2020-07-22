#include "stm32l1xx.h"

class NODEBOX_RF868
{
public:
  void Init(void);

  // SDMMC
  void testSDMMC(char *buffer);   
  
  // W25Q
  void testW25Q(void);     
  
  // LCD
  void testLCD(void);   
  
  //HC05
  void testHC05(void);
   
  // PCF8574
  void testIOEXP8(void);      
  
  // GSM
  void testGSM(void);
  
  // LORA
  void testLORA(void);

};

extern NODEBOX_RF868 node;