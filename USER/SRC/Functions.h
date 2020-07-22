#include "stm32l1xx.h"

#ifndef FUNCTIONS_H
  #define FUNCTIONS_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void delay_ms(unsigned int x);
void delay_us(unsigned int x);
unsigned long htoul(const char *str);
uint16_t millis(void);
char * strchrnul(char *s, int c);
void clearbuffer(char *data, unsigned int size);

void startOperation(unsigned long timeout);
bool isOperationTimedOut(void);
unsigned long operationDuration(void);
uint32_t HAL_GetTick(void); // NO FUNCTIONED DEFINED

#endif