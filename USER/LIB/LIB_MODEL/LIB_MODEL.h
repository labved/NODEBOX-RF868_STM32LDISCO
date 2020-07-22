#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef _USE_GSM
  #define _USE_GSM
#endif

#if(_USE_GSM==1)

#include "Shortcodes.h"
#include "Variables.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class LIB_MODEL   
{
    public:
        void Init(void);
        void test(void);

};






#endif