#include "stm32l1xx.h"  
#include "Settings.h"


#ifndef SIM800C_TYPEDEF_H
  #define SIM800C_TYPEDEF_H


/************************************************************************
***************************ENUMERATION***********************************
************************************************************************/

// RTOS
typedef enum  	 {
  osPriorityIdle = -3,
  osPriorityLow = -2,
  osPriorityBelowNormal = -1,
  osPriorityNormal = 0,
  osPriorityAboveNormal = +1,
  osPriorityHigh = +2,
  osPriorityRealtime = +3,
  osPriorityError = 0x84
}osPriority;


// STATUS - TONE
typedef enum
{
  Sim80xTone_DialTone=1,
  Sim80xTone_CalledSubscriberBusy=2,
  Sim80xTone_Congestion=3,
  Sim80xTone_RadioPathAcknowledge=4,
  Sim80xTone_RadioPathNotAvailable_CallDropped=5,
  Sim80xTone_Error_SpecialInformation=6,
  Sim80xTone_CallWaitingTone=7,
  Sim80xTone_RingingTone=8,
  Sim80xTone_GeneralBeep=16,
  Sim80xTone_PositiveAcknowledgementTone=17,
  Sim80xTone_NegativeAcknowledgementOrErrorTone=18,
  Sim80xTone_IndianDialTone=19,
  Sim80xTone_AmericanDialTone=20,
}Sim80xTone_t;

// STATUS - WAVE
typedef enum
{
  Sim80xWave_Idle=0,
  Sim80xWave_Recording,
  Sim80xWave_Playing,
  
}Sim80xWave_t;

// STATUS - VOICE
typedef enum
{
  GsmVoiceStatus_Idle,
  GsmVoiceStatus_ReturnError,
  GsmVoiceStatus_ReturnNoDialTone,
  GsmVoiceStatus_ReturnNoCarrier,
  GsmVoiceStatus_ReturnNoAnswer,
  GsmVoiceStatus_ReturnBusy,
  GsmVoiceStatus_IAnswerTheCall,
  GsmVoiceStatus_MyCallAnswerd,
  GsmVoiceStatus_Ringing,
  GsmVoiceStatus_Calling,
  
}GsmVoiceStatus_t;

// STATUS - CHARACTERSET

typedef enum
{
  GsmTECharacterSet_Error,
	GsmTECharacterSet_GSM,
	GsmTECharacterSet_UCS2,
	GsmTECharacterSet_IRA,
	GsmTECharacterSet_HEX,
	GsmTECharacterSet_PCCP,
	GsmTECharacterSet_PCDN,
	GsmTECharacterSet_8859_1,	
	
}GsmTECharacterSet_t;

// STATUS - MEMORY
typedef enum
{
  GsmMsgMemory_Error,
  GsmMsgMemory_OnSim,
  GsmMsgMemory_OnModule,
	
}GsmMsgMemory_t;

// STATUS - MESSAGE FORMAT
typedef enum
{
  GsmMsgFormat_Error,
  GsmMsgFormat_PDU,
  GsmMsgFormat_Text,
	
}GsmMsgFormat_t;

//************************BLUETOOTH*************************************************************************************//
// BLUETOOTH TYPE STATUS
typedef enum
{
  BluetoothProfile_NotSet=0,
  BluetoothProfile_GAP,
  BluetoothProfile_SDAP,
  BluetoothProfile_SSP,
  BluetoothProfile_GOEP,
  BluetoothProfile_OPP,
  BluetoothProfile_HSP_HFP,
  BluetoothProfile_A2DP,  
  
}BluetoothProfile_t;

// BLUETOOTH CONNECTION STATUS
typedef enum
{
  BluetoothStatus_Error=-1,
	BluetoothStatus_Initial=0,
	BluetoothStatus_Disactivating=1,
	BluetoothStatus_Activating=2,
	BluetoothStatus_Idle=5,
	BluetoothStatus_Scanning=6,
	BluetoothStatus_Inquiry_Res_Ind=7,
	BluetoothStatus_StoppingScanning=8,
	BluetoothStatus_Bonding=9,
	BluetoothStatus_Connecting=12,
	BluetoothStatus_Unpairing=13,
	BluetoothStatus_DeletingPairedDevice=14,
	BluetoothStatus_DeletingAllPairedDevice=15,
	BluetoothStatus_Disconnecting=16,
	BluetoothStatus_PairingConfirmWhilePassivePairing=19,
	BluetoothStatus_WaitingForRemoteConfirmWhilePassivePairing=20,
	BluetoothStatus_AcceptingConnection=25,
	BluetoothStatus_SDC_Refreshing=26,
	BluetoothStatus_SettingHostName=29,
	BluetoothStatus_ReleasingAllConnection=30,
	BluetoothStatus_ReleasingConnection=31,
	BluetoothStatus_ActivatingService=36,
	
}BluetoothStatus_t;

//************************GPRS*****************************************************************************************//

// GPRS CONNECTION STATUS
typedef enum
{
  GPRSConnection_Idle=0,
  GPRSConnection_AlreadyConnect,
  GPRSConnection_ConnectOK,
  GPRSConnection_ConnectFail,    
  
}GPRSConnection_t;

// GPRS TX RX STATUS
typedef enum
{
  GPRSSendData_Idle=0,
  GPRSSendData_SendOK,
  GPRSSendData_SendFail,
  
}GPRSSendData_t;

// GPRS METHOD STATUS
typedef enum
{
  GPRSHttpMethod_GET=0,
  GPRSHttpMethod_POST=1,
  GPRSHttpMethod_HEAD=2,
  GPRSHttpMethod_DELETE=3,
  
}GPRSHttpMethod_t;


extern osPriority                      GSM_OSPRIORITY;
extern Sim80xTone_t                    GSM_TONE;
extern Sim80xWave_t                    GSM_WAVE;
extern GsmVoiceStatus_t                GSM_VOICE_STATUS;
extern GsmTECharacterSet_t             GSM_TECharacterSet_t ;
extern GsmMsgMemory_t                  GSM_GSMMESSAGEMEMORY;
extern GsmMsgFormat_t                  GSM_GSMMESSAGEFORMAT;
extern BluetoothProfile_t              GSM_BLUETOOTHPROFILE;
extern BluetoothStatus_t               GSM_BLUETOOTHSTATUS;
extern GPRSConnection_t                GSM_GPRSCOMMECTION;
extern GPRSSendData_t                  GSM_GPRSSENDDATA;
extern GPRSHttpMethod_t                GSM_GPRSHTTPMETHORD;


/********************VARIABLES*************************************************/


#endif