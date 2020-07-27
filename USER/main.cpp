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

//test

int main()
{
    __enable_interrupt();
    // System Clock, GPIO Initialise
    node.Init();
    
    printf("Hello World");

    //node.testW25Q();
    //node.testSDMMC(msg);
    //node.testLCD();
    node.testHC05();
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

/* 

3GPP TS 27.007 (GENERIC) 
AT+CGMI           getManNo
AT+CGMM           getModelNo
AT+CGSN           getIMEI
AT+CSCS  NEW ->                   setTeChar,       getTeChar
AT+CSTA  new ->                   setAddrsType,    getAddrsType
AT+CIMI new ->                    getIMSI
AT+COPS new -> ,                  setSelOperator   getSelOperator
AT+CREG new ->                    getNetworkReg,                      command is already used in buffer setting 
AT+CRLP new ->                    setRLPParams     getRLPParams, 
AT+CSQ  new ->                    getCSQ                              command is already used in buffer setting 
AT+CPOL new -> ,                  setPrefOperLst   getPrefOperLst 
AT+COPN new ->                    getOperName
AT+CFUN new ->                    setPhoneFunc     getPhoneFunc,
AT+CCLK new -> ,                  setClockData     getClockData  
AT+CSIM new ->                    setSimAccess 
AT+CBC  new ->                    getBattChar                         command is already used in buffer setting 

// 3GPP TS 27.005 -> only name change & ordering

//SMS
AT+CMGD gsm_deleteMSG->                         deleteSmsMsg
AT+CMGF Gsm_getMsgFormat ->                     getSmsFormat
AT+CMGL
AT+CMGR Gsm_readMsg  ->                         getSmsMsg
AT+CMGS Gsm_sendMsgText ->                      setSmsMsgTxt
AT+CMGW
AT+CMSS
AT+CNMI
AT+CPMS Gsm_getMsgMemoryStatus ->               getSmsMsgMemorySts
AT+CRES
AT+CSAS
AT+CSCA Gsm_setMsgServiceNumber ->              setSmsMsgServiceNo
AT+CSCB
AT+CSDH
AT+CSMP Gsm_getMsgTextModeParameter ->          getSmsMsgTextModeParam
AT+CSMS

//GPRS
AT+CGATT
AT+CGDCONT
AT+CGQMIN
AT+CGQREQ
AT+CGACT
AT+CGDATA
AT+CGPADDR
AT+CGCLASS
AT+CGEREP
AT+CGREG
AT+CGSMS


// TCP/IP (NETWORK) -> only name change & ordering
AT+CIPMUX GPRS_getMultiConnection, GPRS_setMultiConnection -> getNetMultiConnection, setNetMultiConnection
CIPSTART
CIPSEND
CIPQSEND
CIPACK
CIPCLOSE
CIPSHUT GPRS_deactivatePDPContext -> deactivateNetPDPContext
CLPORT
CSTT  GPRS_getAPN, GPRS_setAPN  -> getNetAPN, setNetAPN
CIICR   GPRS_startUpGPRS        -> startupNetGPRS
CIFSR GPRS_getLocalIP           -> getNetLocalIP
CIPSTATUS GPRS_getCurrentConnectionStatus -> getNetCurrentConnectionSts
CDNSCFG
CDNSCFG
CDNSGIP
CIPHEAD
CIPSPRT
CIPSERVER
CIPCSGP
CIPSRIP
CIPDPDP
CIPMODE
CIPCCFG
CIPSHOWTP
SIPUDPMODE
CIPRXGET
CIPSCONT
CIPRDTIMER
CIPSGTXT
CIPTKA

// IP (NETWORK)
SAPBR GPRS_getHttp  -> getHttp

//PING
CIPPING
CIPCTL
CIPFLT
CIPBEIPING

//HTTP -> new write
AT+HTTPINIT                                 httpInit()
AT+HTTPTERM                                 httpTerminate
AT+HTTPPARA                                 getHttpParam, setHttpParam
AT+HTTPDATA                                 getHttpData, setHttpData
AT+HTTPACTION                               getHttpAction, setHttpAction
AT+HTTPREAD                                 getHttpRead, setHttpRead
AT+HTTPCONT                                 getHttpContext
AT+HTTPSTATUS                               getHttpStatus
AT+HTTPHEAD                                 getHttpHeader

// GPS
CIPGSMLOC


// TO BE DELETED

  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CSTA=%d\r\n", type); 
  sim800c.sendAtCommand(temp,200, 0);

*/

