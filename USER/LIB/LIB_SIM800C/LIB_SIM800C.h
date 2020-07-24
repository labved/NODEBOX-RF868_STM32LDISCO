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
    void setActiveProfile(void);
    void setDefaultConfig(void);

    // 0.25V
    void                    setFactoryDefault(void);
    void                    setAtEcho(bool turnon);
    void                    setBaudRate(uint16_t baudrate);
    void                    setPower(bool turnon);      //need to check
    void                    setParameters(void);

    // 3GPP TS 27.007
    void getManNo(void);
    void getModelNo(void);
    void getGlobalNo(void);
    void getLastCommand(void); //need to check
    void getCurrentConfig(void);
    void getTaCapabilities(void);
    void                    getIMEI(char *IMEI);

    // 3GPP TS 27.005


    // SIM800C LIBRARY     
  
   
    // GSM
    void                    Gsm_user(uint32_t startupTime);
    void                    Gsm_userNewCall(const char *CallerNumber); // to be deleted
    void                    Gsm_userNewMsg(char *number,char *Date,char *Time,char *msg);

    bool                    Gsm_ussd(char *send,char *receive);

    bool                    Gsm_callAnswer(void); // to be deleted
    bool                    Gsm_callDisconnect(void); // to be deleted
    GsmVoiceStatus_t        Gsm_dial(char *number,uint8_t WaitForAnswer_second);  // to be deleted
    GsmVoiceStatus_t        Gsm_getLastVoiceActivity(void); // to be deleted
    GsmMsgFormat_t          Gsm_getMsgFormat(void);
    bool                    Gsm_setMsgFormat(GsmMsgFormat_t GsmMsgFormat);  
    GsmMsgMemory_t          Gsm_getMsgMemoryStatus(void);
    bool                    Gsm_setMsgMemoryLocation(GsmMsgMemory_t GsmMsgMemory);
    GsmTECharacterSet_t     Gsm_getMsgCharacterFormat(void);  
    bool                    Gsm_setMsgCharacterFormat(GsmTECharacterSet_t GsmTECharacterSet);
    bool                    Gsm_readMsg(uint8_t index);
    bool                    Gsm_deleteMsg(uint8_t index);
    bool                    Gsm_getMsgServiceNumber(void);
    bool                    Gsm_setMsgServiceNumber(char *ServiceNumber);
    bool                    Gsm_getMsgTextModeParameter(void);
    bool                    Gsm_setMsgTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs);
    bool                    Gsm_sendMsgText(char *number,char *msg);  

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
    bool                    GPRS_deactivatePDPContext(void);
    bool                    GPRS_getAPN(char *Name,char *username,char *password);
    bool                    GPRS_setAPN(char *Name,char *username,char *password);
    bool                    GPRS_startUpGPRS(void);
    void                    GPRS_getLocalIP(char *IP);
    void                    GPRS_getCurrentConnectionStatus(void);
    bool                    GPRS_getMultiConnection(void);
    bool                    GPRS_setMultiConnection(bool Enable);


    void                    GPRS_userHttpGetAnswer(char *data,uint32_t StartAddress,uint16_t dataLen);
    bool                    GPRS_connectToNetwork(char *Name,char *username,char *password,bool EnableMultiConnection);
    bool                    GPRS_getHttp(char *URL); 
    
// UPDATE FUNCTIONS
    bool updateManNo(uint16_t addrs);
    bool updateModelNo(uint16_t addrs);
    bool updateGlobalNo(uint16_t addrs);
    bool updateLastCommand(uint16_t addrs);
    bool updateCurrentConfig(uint16_t addrs);
    bool updateTaCapabilities(uint16_t addrs);
    bool updateIMEI(uint16_t addrs);

};

extern LIB_SIM800C gsm;

#endif

#endif

//
