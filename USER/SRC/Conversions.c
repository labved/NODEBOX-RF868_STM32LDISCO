#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "string.h"

#ifdef CONVERSIONS_H


// CONVERSION ROUTINES
/**
  * @brief  Convert BCD to Decimal
  *         Typically used for RTC Register to Variable
  * @param  bcdByte (byte)
  * @retval Decimal (uint8_t)
  */
uint8_t BCDToDecimal(uint8_t bcdByte)
{
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

/**
  * @brief  Convert Decimal to Byte
  *         Typically used for Variable to RTC Register
  * @param  decimalByte (byte)
  * @retval bcdByte (uint8_t)
  */
uint8_t DecimalToBCD(uint8_t decimalByte)
{
  return (((decimalByte / 10) << 4) | (decimalByte % 10));
}

/**
  * @brief  Convert ASCII to BYTE
  *         Typically used for converting incoming ASCII to Variable Register
  * @param  ascii_temp (byte)
  * @retval byte (uint8_t)
  */
byte ascii2byte(byte value)
{

  uint8_t temp;
  byte returnval;

  temp = (uint8_t)value;

  if ((temp >= 48) & (temp <= 57))
  {
    temp = temp - 48;
  }
  else if ((temp >= 65) & (temp <= 90))
  {
    temp = temp - 60;
  }

  returnval = (byte)temp;

  return returnval;
}

/**
  * @brief  Convert BYTE to ASCII
  *         Typically used for converting BYTE to ASCII before TX
  * @param  value (byte)
  * @retval byte (uint8_t)
  */
byte byte2ascii(byte value)
{

  uint8_t temp;
  byte returnval;

  temp = (uint8_t)value;

  if ((temp >= 48) & (temp <= 57))
  {
    temp = temp - 48;
  }
  else if ((temp >= 65) & (temp <= 90))
  {
    temp = temp - 60;
  }

  returnval = (byte)temp;

  return returnval;
}
// CUSTOM COPY FUNCTIONS

/**
  * @brief  Copy buffer2 to specified len buffer1[0] starting from j of buffer2 
  *         Typically used for copying tmpBuffer to Variable Register
  * @param  recipent_buffer1 (byte), donor_buffer2 (byte), from_positionJ (uint8_t), len (uint8_t) 
  * @retval position_buffer2 (uint8_t)
  */
uint8_t copyBytes(byte *buffer1, byte *buffer2, uint8_t j, uint8_t len)
{
  uint8_t i = 0;

  do
  {
    buffer1[i++] = buffer2[j++];

  } while (i < len);

  return j;
}

/**
  * @brief  Copy entire len of buffer2 to buffer1[0] starting from j of buffer2 
  *         Typically used for copying Variable Register to txBuffer or tmpBuffer
  * @param  recipent_buffer1 (byte), donor_buffer2 (byte), from_positionJ (uint8_t), len (uint8_t) 
  * @retval position_buffer1 (uint8_t)
  */
uint8_t appendBytes(byte *buffer1, byte *buffer2, uint8_t i, uint8_t len)
{
  uint8_t j = 0;

  do
  {
    buffer1[i++] = buffer2[j++];
  } while (j < len);

  return i;
}

#endif