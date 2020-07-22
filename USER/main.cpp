#include "stm32l1xx.h"

#include "Settings.h"
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "stdint.h"
#include "stdio.h"
#include "intrinsics.h"

#include "LIB_NODEBOXRF868.h"
#include "LIB_MODEL.h"


NODEBOX_RF868 node;
LIB_MODEL gen;
char msg[255] = "TESTING";

//testing githhub
//Viveks inputs

int main()
{
    __enable_interrupt();
    // System Clock, GPIO Initialise
    node.Init();
    
    printf("Hello World");

    //node.testW25Q();
    //node.testSDMMC(msg);
    //node.testLCD();
    //node.testHC05();
    //node.testIOEXP8();
    node.testGSM();
    //node.testLORA();
    //gen.test();
    
    while(1)
    {
      GPIO_WriteBit(LED_GPIO, LEDON_PIN, Bit_SET);
      delay_ms(1000);
      GPIO_WriteBit(LED_GPIO, LEDON_PIN, Bit_RESET);
      delay_ms(1000);
      printf("Hello World\r\n");

    }
   
}




