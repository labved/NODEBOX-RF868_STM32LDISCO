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

//*************************************** PCF8574  **************************************************//

// ADDRESS
#define PCF8574_DEV0_ADDR					0x00
#define PCF8574_DEV1_ADDR					0x01
#define PCF8574_DEV2_ADDR					0x02
#define PCF8574_DEV3_ADDR					0x03
#define PCF8574_DEV4_ADDR					0x04
#define PCF8574_DEV5_ADDR					0x05
#define PCF8574_DEV6_ADDR					0x06
#define PCF8574_DEV7_ADDR					0x07

// HW PINS
#define PCF8574_DEV0_GPIOA_PIN0				0x00
#define PCF8574_DEV0_GPIOA_PIN1				0x01
#define PCF8574_DEV0_GPIOA_PIN2				0x02
#define PCF8574_DEV0_GPIOA_PIN3				0x03
#define PCF8574_DEV0_GPIOA_PIN4				0x04
#define PCF8574_DEV0_GPIOA_PIN5				0x05
#define PCF8574_DEV0_GPIOA_PIN6				0x06
#define PCF8574_DEV0_GPIOA_PIN7				0x07

#define PCF8574_DEV0_GPIOA_PIN0				0x00
#define PCF8574_DEV0_GPIOA_PIN1				0x01
#define PCF8574_DEV0_GPIOA_PIN2				0x02
#define PCF8574_DEV0_GPIOA_PIN3				0x03
#define PCF8574_DEV0_GPIOA_PIN4				0x04
#define PCF8574_DEV0_GPIOA_PIN5				0x05
#define PCF8574_DEV0_GPIOA_PIN6				0x06
#define PCF8574_DEV0_GPIOA_PIN7				0x07

// MASK
#define PCF8574_MASK_PIN0					0x01  
#define PCF8574_MASK_PIN1					0x02  
#define PCF8574_MASK_PIN2					0x04  
#define PCF8574_MASK_PIN3					0x08  
#define PCF8574_MASK_PIN4					0x10  
#define PCF8574_MASK_PIN5					0x20  
#define PCF8574_MASK_PIN6					0x40  
#define PCF8574_MASK_PIN7					0x80

// MASK COUNT
#define PCF8574_MASK_COUNT_PIN0				0 
#define PCF8574_MASK_COUNT_PIN1				1  
#define PCF8574_MASK_COUNT_PIN2				2  
#define PCF8574_MASK_COUNT_PIN3				3  
#define PCF8574_MASK_COUNT_PIN4				4  
#define PCF8574_MASK_COUNT_PIN5				5  
#define PCF8574_MASK_COUNT_PIN6				6  
#define PCF8574_MASK_COUNT_PIN7				7

//*************************************** HC05 **************************************************//

#define AT_INITIALIZED      "INITIALIZED"
#define AT_READY                     "READY"
#define AT_PAIRABLE     "PAIRABLE"
#define AT_PAIRED     "PAIRED"
#define AT_INQUIRING     "INQUIRING"
#define AT_CONNECTING     "CONNECTING"
#define AT_CONNECTED     "CONNECTED"
#define AT_DISCONNECTED     "DISCONNECTED"
#define AT_UNKNOWN     "UNKNOWN"

#define AT_SUCCESS     "SUCCESS"
#define AT_LINK_LOSS     "LINK_LOSS"
#define AT_NO_SLC     "NO_SLC"
#define AT_TIMEOUT     "TIMEOUT"
#define AT_ERROR     "ERROR"
#define AT_EOL    "\r\n"


#define AT_RESET     "RESET"

#define AT_ORGL     "ORGL"
#define AT_CMD_ENSNIFF    "ENSNIFF"
#define AT_CMD_EXSNIFF     "EXSNIFF"
#define AT_BIND     "BIND"

#define AT_CMD_GET_VERSION    "VERSION?"
#define AT_RESP_VERSION     "+VERSION:"


#define AT_DISC     "DISC"
#define AT_CMD_GET_DISC     "DISC?"
#define AT_CMD_SET_DISC     "DISC="
#define AT_RESP_DISC     "+DISC:"

#define AT_ADDR             "ADDR"
#define AT_CMD_GET_ADDR     "ADDR?"
#define AT_CMD_SET_ADDR     "ADDR="
#define AT_RESP_ADDR        "+ADDR:"


#define AT_CMD_GET_NAME     "NAME?"
#define AT_CMD_SET_NAME     "NAME="
#define AT_RESP_NAME        "+NAME:"

#define AT_CMD_GET_RNAME     "RNAME?"
#define AT_CMD_SET_RNAME     "RNAME="
#define AT_RESP_RNAME        "+RNAME:"

#define AT_CMD_GET_ROLE     "ROLE?"
#define AT_CMD_SET_ROLE     "ROLE="
#define AT_RESP_ROLE        "+ROLE:"

#define AT_CMD_GET_CLASS     "CLASS?"
#define AT_CMD_SET_CLASS    "CLASS="
#define AT_RESP_CLASS        "+CLASS:"

#define AT_CMD_GET_IAC     "IAC?"
#define AT_CMD_SET_IAC    "IAC="
#define AT_RESP_IAC        "+IAC:"

#define AT_CMD_GET_INQM     "INQM?"
#define AT_CMD_SET_INQM    "INQM="
#define AT_RESP_INQM        "+INQM:"

#define AT_CMD_GET_PSWD     "PSWD?"
#define AT_CMD_SET_PSWD    "PSWD="
#define AT_RESP_PSWD        "+PSWD:"

#define AT_CMD_GET_UART     "UART?"
#define AT_CMD_SET_UART    "UART="
#define AT_RESP_UART        "+UART:"

#define AT_CMD_GET_CMODE     "CMODE?"
#define AT_CMD_SET_CMODE    "CMODE="
#define AT_RESP_CMODE        "+CMODE:"

#define AT_CMD_GET_POLAR     "POLAR?"
#define AT_CMD_SET_POLAR    "POLAR="
#define AT_RESP_POLAR        "+POLAR:"

#define AT_CMD_GET_PIO     "PIO?"
#define AT_CMD_SET_PIO    "PIO="
#define AT_RESP_PIO        "+PIO:"

#define AT_CMD_GET_MPIO     "MPIO?"
#define AT_CMD_SET_MPIO    "MPIO="
#define AT_RESP_MPIO        "+MPIO:"

#define AT_CMD_GET_IPSCAN     "IPSCAN?"
#define AT_CMD_SET_IPSCAN    "IPSCAN="
#define AT_RESP_IPSCAN        "+IPSCAN:"

#define AT_CMD_GET_SNIFF     "SNIFF?"
#define AT_CMD_SET_SNIFF    "SNIFF="
#define AT_RESP_SNIFF       "+SNIFF:"

#define AT_CMD_GET_SENM     "SENM?"
#define AT_CMD_SET_SENM    "SENM="
#define AT_RESP_SENM       "+SENM:"

#define AT_RMSAD             "RMSAD"
#define AT_RMAAD             "RMAAD"
#define AT_FSAD             "FSAD"
#define AT_MRAD             "MRAD"

#define AT_CMD_GET_ADCN     "ADCN?"
#define AT_CMD_SET_ADCN     "ADCN="
#define AT_RESP_ADCN        "+ADCN:"

#define AT_CMD_GET_STATE     "STATE?"
#define AT_CMD_SET_STATE     "STATE="
#define AT_RESP_STATE        "+STATE:"

#define AT_INIT     "INIT"

#define AT_INQ     "INQ"
#define AT_CMD_GET_INQ      "INQ?"
#define AT_CMD_SET_INQ      "INQ="
#define AT_RESP_INQ         "+INQ:"

#define AT_INQC     "INQC"
#define AT_PAIR     "PAIR"


#endif