#include "stm32l1xx.h" 
#include "Shortcodes.h"

#ifndef CONVERSIONS_H
  #define CONVERSIONS_H

// DATA TYPE CONVERSIONS
uint8_t BCDToDecimal(uint8_t bcdByte);
uint8_t DecimalToBCD(uint8_t decimalByte);
byte ascii2byte(byte value);
byte byte2ascii(byte value);

char *itoa(int32_t i, byte *a, int32_t r );
char *i2a(unsigned i, byte *a, unsigned r);

// CUSTOM COPY FUNCTIONS
uint8_t copyBytes(byte *buffer1, byte *buffer2, uint8_t j, uint8_t len);
uint8_t appendBytes(byte *buffer1, byte *buffer2, uint8_t i, uint8_t len);

#endif