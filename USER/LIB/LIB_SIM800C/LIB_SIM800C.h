#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef LIB_SIM800C_H
  #define LIB_SIM800C_H

#if(_USE_GSM==1)

class LIB_SIM800C   
{
  private:
    void                    PoweronReset(void);
    void                    RegConfigSettings(void);
    void                    debugTerminal(char *msg);

  public:
    void                    test(void);  // to be deleted
    void                    Init(void);
    void                    softReset(void);
    bool                    probe(unsigned long timeout );
    void	                  rxCallBack(void);
    void                    startSim80xBuffTask(void const * argument);
    void                    bufferProcess(void);
    void                    startSim80xTask(void const * argument);


    // CUSTOM LIBRARY
    void setAtMode(void); //need to check
    void setDataMode(void);
    void setActiveProfile(void); //need to check
    void setDefaultConfig(void);

    // 0.25V
    void                    setFactoryDefault(void);
    void                    setAtEcho(bool turnon);
    void                    setBaudRate(uint16_t baudrate);
    void                    setPower(bool turnon);      //need to check
    void                    setParameters(void);

    // 3GPP TS 27.007
    bool getManNo(void);
    bool getModelNo(void);
    bool getGlobalNo(void);
    bool getLastCommand(void); //need to check
    bool getCurrentConfig(void);
    bool getTaCapabilities(void);
    bool                    getIMEI(char *IMEI);
    
    bool getTeChar(void);
    bool getAddrsType(void);
    bool getIMSI(void);
    bool getOperator(void);
    bool getNetworkReg(void);
    bool getRLPParams(void);
    bool getCSQ(void);
    bool getPrefOperLst(void);   
    bool getOperName(void);
    bool getPhoneFunc(void);
    bool getClockData(void);
    bool getBattChar(void);

    
    void setTeChar(char *chset);
    void setAddrsType(uint8_t type);
    void setSelOperator(uint8_t mode, uint8_t format, uint8_t oper); //need to check
    void setNetworkReg(bool turnon);
    void setRLPParams(uint16_t iws,uint16_t mws,uint16_t t2,uint16_t n2,uint16_t t4);
    void setPrefOperLst(uint8_t index, uint8_t  format, char *oper);  //need to check
    void setPhoneFunc(uint8_t fun, uint8_t  rst);
    void setClockData(char time);
    void setSimAccess(uint16_t length, char command);



    // 3GPP TS 27.005


    // SIM800C LIBRARY     
  
   
    // GSM
    void                    Gsm_user(uint32_t startupTime);
    void                    Gsm_userNewMsg(char *number,char *Date,char *Time,char *msg);

    bool                    Gsm_ussd(char *send,char *receive);

    GsmMsgFormat_t          getSmsFormat(void);
    bool                    Gsm_setMsgFormat(GsmMsgFormat_t GsmMsgFormat);  
    GsmMsgMemory_t          getSmsMsgMemorySts(void);
    bool                    Gsm_setMsgMemoryLocation(GsmMsgMemory_t GsmMsgMemory);
    GsmTECharacterSet_t     Gsm_getMsgCharacterFormat(void);  
    bool                    Gsm_setMsgCharacterFormat(GsmTECharacterSet_t GsmTECharacterSet);
    bool                    getSmsMsg(uint8_t index);
    bool                    deleteSmsMsg(uint8_t index);
    bool                    getSmsMsgServiceNumber(void);
    bool                    setSmsMsgServiceNo(char *ServiceNumber);
    bool                    getSmsMsgTextModeParam(void);
    bool                    Gsm_setMsgTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs);
    bool                    setSmsMsgTxt(char *number,char *msg);  

// BLUETOOTH
    void                    Bluetooth_userNewPairingRequest(char *Name,char *Address,char *Pass);
    void                    Bluetooth_userConnectingSpp(void);
    void                    Bluetooth_userNewSppData(char *NewData,uint16_t len);
   
    bool                    Bluetooth_setPower(bool turnon);
    bool                    Bluetooth_getHostName(void);  
    bool                    Bluetooth_setHostName(char *HostName);
    BluetoothStatus_t       Bluetooth_getStatus(void);
    bool                    Bluetooth_acceptPair(bool Accept);  
    bool                    Bluetooth_acceptPairWithPass(char *Pass);  
    bool                    Bluetooth_setAutoPair(bool  Enable);
    bool                    Bluetooth_setPairPassword(char  *Pass);
    bool                    Bluetooth_unpair(uint8_t  Unpair_0_to_all);  
    bool                    Bluetooth_getVisibility(void);
    void                    Bluetooth_setVisibility(bool Visible);
    void                    Bluetooth_sppAllowConnection(bool Accept);
    bool                    Bluetooth_sppSend(char *DataString);

// GPRS
    bool                    deactivateNetPDPContext(void);
    bool                    getNetAPN(char *Name,char *username,char *password);
    bool                    setNetAPN(char *Name,char *username,char *password);
    bool                    startupNetGPRS(void);
    void                    getNetLocalIP(char *IP);
    void                    getNetCurrentConnectionSts(void);
    bool                    getNetMultiConnection(void);
    bool                    setNetMultiConnection(bool Enable);


    void                    GPRS_userHttpGetAnswer(char *data,uint32_t StartAddress,uint16_t dataLen);
    bool                    GPRS_connectToNetwork(char *Name,char *username,char *password,bool EnableMultiConnection);
    bool                    getHttp(char *URL); 
    
// UPDATE FUNCTIONS
    bool processManNo(uint16_t addrs);
    bool processModelNo(uint16_t addrs);
    bool processGlobalNo(uint16_t addrs);
    bool processLastCommand(uint16_t addrs);
    bool processCurrentConfig(uint16_t addrs);
    bool processTaCapabilities(uint16_t addrs);
    bool processNetworkReg(uint16_t addrs);
    
    void processCSQ(uint16_t addrs);//build
    void processCBC(uint16_t addrs);

    void processSmsMsgTxt(char *number, 
    char *msg, uint16_t addrs);
    void processSmsMsgMemorySts(uint16_t addrs);
    void processSmsMsg(uint16_t addrs);
    void processSmsMsgServiceNo(uint16_t addrs);//done
    void processSmsTextModeParam(uint16_t addrs);//done
    void processBTGetHostName(uint16_t addrs);//done
    void processBTStatus(uint16_t addrs);//done
    void processBTPair(uint16_t addrs);//done
    void processBTVisibility(uint16_t addrs);//done
    void processGPRSNetApn(uint16_t addrs);//done
    void processGPRSNetLocalIP(uint16_t addrs);//done
    void processGPRSNetMultiConnection(uint16_t addrs);//done
    void processIMEI(uint16_t addrs);


    //HTTP

    
    void httpInit(void);
    void httpTerminate(void);
    void getHttpParam(void);
    void setHttpParam(void);
    void getHttpData(void);
    void setHttpData(void);
    void getHttpAction(void);
    void setHttpAction(void);
    void getHttpRead(void);
    
    void setHttpRead(void);
    void getHttpContext(void);
    void getHttpStatus(void);
    void getHttpHeader(void);

};

extern LIB_SIM800C gsm;

#endif

#endif

//
