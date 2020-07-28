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
    void                    debugTerminal(char const *msg);

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
    bool                    setAtMode(void); //need to check
    bool                    setDataMode(void);
    bool                    setActiveProfile(void); //need to check
    bool                    setDefaultConfig(void);

    // 0.25V
    bool                    setFactoryDefault(void);
    bool                    setAtEcho(bool turnon);
    bool                    setBaudRate(uint16_t baudrate);
    bool                    setPower(bool turnon);      //need to check //made no changes
    bool                    setParameters(void);

    // 3GPP TS 27.007
    bool                    getManNo(void);
    bool                    getModelNo(void);
    bool                    getGlobalNo(void);
    bool                    getLastCommand(void); //need to check
    bool                    getCurrentConfig(void);
    bool                    getTaCapabilities(void);
    bool                    getIMEI(char *IMEI);
    
    bool                    getTeChar(void);
    bool                    getAddrsType(void);
    bool                    getIMSI(void);
    bool                    getOperator(void);
    bool                    getNetworkReg(void);
    bool                    getRLPParams(void);
    bool                    getCSQ(void);
    bool                    getPrefOperLst(void);   
    bool                    getOperName(void);
    bool                    getPhoneFunc(void);
    bool                    getClockData(void);
    bool                    getBattChar(void);

    
    bool                    setTeChar(char *chset);
    bool                    setAddrsType(uint8_t type);
    bool                    setSelOperator(uint8_t mode, uint8_t format, uint8_t oper); //need to check
    bool                    setNetworkReg(bool turnon);
    bool                    setRLPParams(uint16_t iws,uint16_t mws,uint16_t t2,uint16_t n2,uint16_t t4);
    bool                    setPrefOperLst(uint8_t index, uint8_t  format, char *oper);  //need to check
    bool                    setPhoneFunc(uint8_t fun, uint8_t  rst);
    bool                    setClockData(char time);
    bool                    setSimAccess(uint16_t length, char command);



    // 3GPP TS 27.005
    // SIM800C LIBRARY     
  
    // GSM
    void                    smsUser(uint32_t startupTime);
    void                    smsUserNewMsg(char *number,char *Date,char *Time,char *msg);

    bool                    smsUssd(char *send,char *receive);

    GsmMsgFormat_t          getSmsFormat(void);
    bool                    setSmsMsgFormat(GsmMsgFormat_t GsmMsgFormat);  
    GsmMsgMemory_t          getSmsMsgMemorySts(void);
    bool                    setSmsMsgMemoryLocation(GsmMsgMemory_t GsmMsgMemory);
    GsmTECharacterSet_t     getSmsMsgCharacterFormat(void);  
    bool                    setSmsMsgCharacterFormat(GsmTECharacterSet_t GsmTECharacterSet);
   
    bool                    getSmsMsg(uint8_t index);
    bool                    deleteSmsMsg(uint8_t index);
    bool                    getSmsMsgServiceNumber(void);
    bool                    setSmsMsgServiceNo(char *ServiceNumber);
    bool                    getSmsMsgTextModeParam(void);
    bool                    setSmsMsgTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs);
    bool                    setSmsMsgTxt(char *number,char *msg);  

// BLUETOOTH
    void                    userBtNewPairingRequest(char *Name,char *Address,char *Pass);
    void                    userBtConnectingSpp(void);
    void                    userBtNewSppData(char *NewData,uint16_t len);
   
    bool                    setBtPower(bool turnon);
    bool                    getBtHostName(void);  
    bool                    setBtHostName(char *HostName);
    BluetoothStatus_t       getBtStatus(void);
    bool                    acceptPair(bool Accept);  
    bool                    acceptPairWithPass(char *Pass);  
    bool                    setBtAutoPair(bool  Enable);
    bool                    setBtPairPassword(char  *Pass);
    bool                    btUnpair(uint8_t  Unpair_0_to_all);  
    bool                    getBtVisibility(void);
    bool                    setBtVisibility(bool Visible);
    bool                    btSppAllowConnection(bool Accept);
    bool                    btSppSend(char *DataString);

// GPRS
    bool                    deactivateNetPDPContext(void);
    bool                    getNetAPN(char *Name,char *username,char *password);
    bool                    setNetAPN(char *Name,char *username,char *password);
    bool                    startupNetGPRS(void);
    bool                    getNetLocalIP(char *IP);
    bool                    getNetCurrentConnectionSts(void);
    bool                    getNetMultiConnection(void);      //need to check
    bool                    setNetMultiConnection(bool Enable);


    void                    GPRS_userHttpGetAnswer(char *data,uint32_t StartAddress,uint16_t dataLen);
    bool                    GPRS_connectToNetwork(char *Name,char *username,char *password,bool EnableMultiConnection);
    bool                    getHttp(char *URL); 
    
// UPDATE FUNCTIONS
    void processManNo(uint16_t addrs);
    void processModelNo(uint16_t addrs);
    void processGlobalNo(uint16_t addrs);
    void processLastCommand(uint16_t addrs);
    void processCurrentConfig(uint16_t addrs);
    void processTaCapabilities(uint16_t addrs);
    void processNetworkReg(uint16_t addrs);
    
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
    
    bool processHttpData(void);
    bool processHttpAction(void);
    bool  processHttpRead(void);
    bool processHttpContent(void);



    //HTTP 
    void httpInit(void);
    void httpTerminate(void);


    bool getHttpParam(void);
    bool getHttpData(void);
    bool getHttpAction(void);
    bool getHttpRead(void);
    bool getHttpContext(void);
    bool getHttpStatus(void);
    bool getHttpHeader(void);

    bool setHttpParam(void);
    bool setHttpData(void);
    bool setHttpAction(void);
    bool setHttpRead(void);
    




};

extern LIB_SIM800C gsm;

#endif

#endif

//
