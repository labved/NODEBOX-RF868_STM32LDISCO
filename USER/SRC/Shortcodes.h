#include "stm32l1xx.h"

#ifndef SHORTCODES_H
  #define SHORTCODES_H

#define byte char
#define uchar unsigned char

/* FRAME BYTE DEFINITION        */
#define XON_BYTE 0x11  // XON - Device Control 1
#define XOFF_BYTE 0x13 // XOFF - Device Control 3
#define DEV_CTL2 0x12  // Device Control 2
#define DEV_CTL4 0x14  // Device Control 4

#define SOH_BYTE 0x01 // SOH - Start of Heading
#define ENQ_BYTE 0x05 // ENQ - Enquiry
#define STX_BYTE 0x02 // STX - Start of Text
#define ETX_BYTE 0x03 // ETX - End of Text
#define EOT_BYTE 0x04 // EOT - End of Transmission

#define INT_BYTE '*'
#define VAL_BYTE '&'
#define SEP_BYTE '-'

/* RELATION OPERATOR INPUTS	*/
#define GREATER 0x3E
#define LESSER 0x3C

/* COMMAND DEFINITION     	*/
#define PROG "PR"
#define SET "ST"
#define GET "GT"
#define ACK "AK"
#define QUIT "QT"

/* WSN TYPE DEFINITION     	*/
#define NOD "D0"
#define SNSR "S0"

/* PARAMETER DEFINITION         */
#define ADRS "P001"
#define NAM "P002"
#define ZONE_NAM "P003"
#define ZONE_ADRS "P004"
#define GPS_LAT "P005"
#define GPS_LON "P006"
#define CLOCK "P007"
#define RELAY_ON "P008"
#define RELAY_OFF "P009"

#define VALUE "P013"
#define BAT_V "P014"

#define TIMER_INTV "P051"
#define COMP_SYM "P052"
#define COMP_VAL "P053"

#endif