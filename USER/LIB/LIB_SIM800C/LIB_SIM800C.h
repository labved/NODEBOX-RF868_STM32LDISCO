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
    void                    PoweronReset(void);//done
    void                    RegConfigSettings(void);
    void                    debugTerminal(char const *msg);

  public:
    void                    test(void);  // to be deleted
    void                    Init(void);
    void                    softReset(void);
    bool                    probe(unsigned long timeout );
    void	                  rxCallBack(void);//need to check
    void                    startSim80xBuffTask(void const * argument);//need to check
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
    bool                    getIMEI(char *IMEI);// need to heck//can use void statement
    
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
    bool                    setClockData(char *time);
    bool                    setSimAccess(uint16_t length, char command);



    // 3GPP TS 27.005
    // SIM800C LIBRARY     
  
    // GSM USER
    void                    smsUser(uint32_t startupTime);
    void                    smsUserNewMsg(char *number,char *Date,char *Time,char *msg);
    // GSM 
    
    bool                    smsUssd(char *send,char *receive);

    GsmMsgFormat_t          getSmsFormat(void);
    GsmMsgMemory_t          getSmsMsgMemorySts(void);
    GsmTECharacterSet_t     getSmsMsgCharacterFormat(void);  
    
    bool                    getSmsMsg(uint8_t index);
    bool                    getSmsMsgServiceNumber(void);
    bool                    getSmsMsgTextModeParam(void);
    
    bool                    setSmsMsgFormat(GsmMsgFormat_t GsmMsgFormat);  
    bool                    setSmsMsgMemoryLocation(GsmMsgMemory_t GsmMsgMemory);
    bool                    setSmsMsgCharacterFormat(GsmTECharacterSet_t GsmTECharacterSet);
       
    bool                    setSmsMsgServiceNo(char *ServiceNumber);
    bool                    setSmsMsgTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs);
    bool                    setSmsMsgTxt(char *number,char *msg);  
    
    bool                    deleteSmsMsg(uint8_t index);
    
// BLUETOOTH USER
    void                    userBtNewPairingRequest(char *Name,char *Address,char *Pass);
    void                    userBtConnectingSpp(void);
    void                    userBtNewSppData(char *NewData,uint16_t len);
// BLUETOOTH   
    bool                    getBtHostName(void);  
    BluetoothStatus_t       getBtStatus(void);
    bool                    getBtVisibility(void);//need to check
    
    bool                    setBtPower(bool turnon);//need to check
    bool                    setBtHostName(char *HostName);//need to check
    bool                    setBtAutoPair(bool  Enable);
    bool                    setBtPairPassword(char  *Pass);
    bool                    setBtVisibility(bool Visible);//need to check
    
    bool                    acceptPair(bool Accept);  
    bool                    acceptPairWithPass(char *Pass);  
    
    bool                    btUnpair(uint8_t  Unpair_0_to_all);  
    bool                    btSppAllowConnection(bool Accept);
    bool                    btSppSend(char *DataString);//need to check


// GPRS USER
    bool                    getUserHttpAnswer(char *data,uint32_t StartAddress,uint16_t dataLen);

// GPRS
    bool                    deactivateNetPDPContext(void);
    
    bool                    getNetAPN(char *Name,char *username,char *password);
    bool                    getNetLocalIP(char *IP);
    bool                    getNetCurrentConnectionSts(void);
    bool                    getNetMultiConnection(void);      //need to check
    bool                    getHttp(char *URL); //need to check
    
    bool                    setNetAPN(char *Name,char *username,char *password);
    bool                    setNetMultiConnection(bool Enable);

    bool                    startupNetGPRS(void);
    
    bool                    connectToNetwork(char *Name,char *username,char *password,bool EnableMultiConnection);  //need to check
    
// UPDATE FUNCTIONS
    bool processManNo(uint16_t addrs);//what to return  //need to check
    bool processModelNo(uint16_t addrs);//what to return  //need to check
    bool processGlobalNo(uint16_t addrs);//what to return  //need to check
    bool processLastCommand(uint16_t addrs);//what to return  //need to check
    bool processCurrentConfig(uint16_t addrs);//what to return  //need to check
    bool processTaCapabilities(uint16_t addrs);//what to return  //need to check
    bool processNetworkReg(uint16_t addrs);
    
    bool processCSQ(uint16_t addrs);//build
    bool processCBC(uint16_t addrs);

    bool processSmsMsgTxt(char *number, 
    char *msg, uint16_t addrs);
    bool processSmsMsgMemorySts(uint16_t addrs);
    bool processSmsMsg(uint16_t addrs);
    bool processSmsMsgServiceNo(uint16_t addrs);//done
    bool processSmsTextModeParam(uint16_t addrs);//done
    
    bool processBTGetHostName(uint16_t addrs);//done
    bool processBTStatus(uint16_t addrs);//done
    bool processBTPair(uint16_t addrs);//done
    bool processBTVisibility(uint16_t addrs);//done
    
    bool processGPRSNetApn(uint16_t addrs);//done
    bool processGPRSNetLocalIP(uint16_t addrs);//done
    bool processGPRSNetMultiConnection(uint16_t addrs);//done
    bool processIMEI(uint16_t addrs);
    
    bool processHttpData(void); //need to check
    bool processHttpAction(void); //need to check
    bool processHttpRead(void); //need to check
    bool processHttpContent(void);  //need to check



    //HTTP 
    bool httpInit(void);
    bool httpTerminate(void);


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
