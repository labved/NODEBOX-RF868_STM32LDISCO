#include "stm32l1xx.h"
#include "Settings.h"


#ifndef _USE_BT
  #define _USE_BT
#endif

#if(_USE_BT==1)

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