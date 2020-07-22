#include "stm32l1xx.h"

#include "Functions.h"
#include "intrinsics.h"

#ifdef FUNCTIONS_H

void delay_ms(unsigned int x)
{
  while (x)
  {
    for (int i = 0; i <= 3200; i++) //(CLK/10000)*2 - 3200 for 16MHz
    {
      __no_operation();
    }
    x--;
  }
}
void delay_us(unsigned int x)
{
  while (x)
  {
    for (int i = 0; i <= 6; i++)
      ; //(CLK/10000000)*2 - 3.2 approx 4 for 16MHz
    x--;
  }
}


uint16_t millis(void)
{
  return 1;
}


// to move to Conversions.cpp
unsigned long htoul(const char *str)
{
  if (!str)
    return 0;

  long result = 0;

  while (*str)
  {
    uint8_t value;

    if (*str >= 'a' && *str <= 'f')
      value = (*str - 'a') + 10;
    else if (*str >= 'A' && *str <= 'F')
      value = (*str - 'A') + 10;
    else if (*str >= '0' && *str <= '9')
      value = *str - '0';
    else
      break;

    result = (result * 16) + value;
    ++str;
  }

  return result;
}
// to move to Conversions.cpp
char * strchrnul(char *s, int c)
{
  char *result = strchr(s, c);
  if (result == NULL)
    result = strchr (s, '\0');
  return result;
}

void clearbuffer(char *data, unsigned int size)
{
  for(unsigned int i=0;i<size;i++)
  {
    *data = '\0';
    data++;
  }
}


/****************************************************************
*FUNCTION NAME:startOperation
*FUNCTION     :startOperation
*INPUT        :timeout; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void startOperation(unsigned long timeout)
{
  /*
   uint16_t m_ticksAtStart = millis();
   m_timeout = timeout;
  */
  
  //m_errCode = HC05_OK;
}


/****************************************************************
*FUNCTION NAME:isOperationTimedOut
*FUNCTION     :timeout
*INPUT        :none
*OUTPUT       :boolean
****************************************************************/
bool isOperationTimedOut(void) 
{
  //return operationDuration() >= m_timeout;
  return 0;
}

/****************************************************************
*FUNCTION NAME:operationDuration
*FUNCTION     :operationDuration
*INPUT        :none
*OUTPUT       :unsigned long
****************************************************************/
unsigned long operationDuration(void) 
{
    /*
  unsigned long current_ticks = millis(), elapsed_ticks;

  if (current_ticks >= m_ticksAtStart)
    elapsed_ticks = current_ticks - m_ticksAtStart;
  else
    elapsed_ticks = (ULONG_MAX - m_ticksAtStart) + current_ticks;

  return elapsed_ticks;
  */
  
  return 0;
}
#endif