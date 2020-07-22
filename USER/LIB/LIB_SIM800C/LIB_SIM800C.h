#include "stm32l1xx.h"  
#include "Settings.h"

#ifndef LIB_SIM800C_H
  #define LIB_SIM800C_H

#if(_USE_GSM==1)

#include "Shortcodes.h"
#include "Variables.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class LIB_SIM800C   
{
  private:
    void PoweronReset(void);
    void RegConfigSettings(void);
    void debugTerminal(char *msg);

  public:
    void                    test(void);  // to be deleted
    void                    Init(void);
    void                    softReset(void);
    bool                    probe(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
    void	                  rxCallBack(void);
    void                    startSim80xBuffTask(void const * argument);
    void                    bufferProcess(void);
    void                    startSim80xTask(void const * argument);

    // SIM800C LIBRARY     
    void                    saveParameters(void);
    void                    setPower(bool TurnOn);
    void                    setFactoryDefault(void);
    void                    setBaudRate(uint16_t baudrate);
    void                    setAtEcho(bool turnon);
    void                    getIMEI(char *IMEI);
    uint8_t                 getRingVol(void);
    bool                    setRingVol(uint8_t Vol_0_to_100);
    uint8_t                 getLoadVol(void);//need to check
    bool                    setLoadVol(uint8_t Vol_0_to_100);
    Sim80xWave_t            waveGetState(void);
    bool                    waveRecord(uint8_t ID,uint8_t TimeLimitInSecond);  
    bool                    wavePlay(uint8_t ID);  
    bool                    waveStop(void);  
    bool                    waveDelete(uint8_t ID);
    bool                    setMicGain(uint8_t Channel_0_to_4,uint8_t Gain_0_to_15);
    bool                    getMicGain(void);
    bool                    tonePlay(Sim80xTone_t Sim80xTone,uint32_t  Time_ms);
    bool                    toneStop(void);
    uint8_t                 getToneVol(void);
    bool                    setToneVol(uint8_t Vol_0_to_100);
    bool                    setRingTone(uint8_t Tone_0_to_19,bool Save);
    bool                    setEchoParameters(uint8_t  SelectMic_0_or_1,uint16_t NonlinearProcessingRemove,uint16_t AcousticEchoCancellation,uint16_t NoiseReduction,uint16_t NoiseSuppression);
   
    // GSM
    void                    Gsm_user(uint32_t StartupTime);
    void                    Gsm_userNewCall(const char *CallerNumber);
    void                    Gsm_userNewMsg(char *Number,char *Date,char *Time,char *msg);

    bool                    Gsm_ussd(char *send,char *receive);

    bool                    Gsm_callAnswer(void);
    bool                    Gsm_callDisconnect(void);
    GsmVoiceStatus_t        Gsm_dial(char *Number,uint8_t WaitForAnswer_second); 
    GsmVoiceStatus_t        Gsm_getLastVoiceActivity(void);
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
    bool                    Gsm_sendMsgText(char *Number,char *msg);  

// BLUETOOTH
    void                    Bluetooth_userNewPairingRequest(char *Name,char *Address,char *Pass);
    void                    Bluetooth_userConnectingSpp(void);
    void                    Bluetooth_userNewSppData(char *NewData,uint16_t len);
   
    bool                    Bluetooth_setPower(bool TurnOn);
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
    

  
};



extern LIB_SIM800C gsm;

#endif

#endif
