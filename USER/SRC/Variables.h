#include "stm32l1xx.h" 

#include "SIM800C_TYPEDEF.h"

#ifndef VARIABLES_H
  #define VARIABLES_H

/* BUFFER DEFINITION            */
extern byte    			        txBuffer[MAX_BUFFLEN];
extern byte 				rxBuffer[MAX_BUFFLEN];
extern byte 				tmpBuffer[MAX_BUFFLEN];

// LED
#define LED_GPIO_InitStruct		        GPIO_LED_InitStruct

// CLOCKS
#define MCO_GPIO_InitStruct		        GPIO_MCO_InitStruct

// RTC
#define RTC_GPIO_InitStruct		        GPIO_RTC_InitStruct
#define RTC_InitStruct		                RTC_InitStruct
#define RTC_Time_InitStruct		        RTC_Time_InitStruct
#define RTC_Date_InitStruct		        RTC_Date_InitStruct

#define RTC_EXTI_InitStruct		        EXTI_RTC_InitStruct


// LORA
#define SPI_LORA_GPIO_InitStruct		GPIO_LORA_SPI_InitStruct
#define SPI_LORA_GPIO_CSn_InitStruct		GPIO_LORA_CSn_InitStruct
#define SPI_LORA_GPIO_GDO0_InitStruct		GPIO_LORA_GDO0_InitStruct
#define SPI_LORA_GPIO_GDO2_InitStruct		GPIO_LORA_GDO2_InitStruct
#define SPI_LORA_GPIO_CLK_InitStruct		GPIO_LORA_CLK_InitStruct
#define SPI_LORA_GPIO_MOSI_InitStruct		GPIO_LORA_MOSI_InitStruct
#define SPI_LORA_GPIO_MISO_InitStruct		GPIO_LORA_MISO_InitStruct

#define SPI_LORA_SPI_InitStruct		        SPI_LORA_InitStruct

#define LORA_EXTI_GDO0_InitStruct		EXTI_GDO0_InitStruct
#define LORA_NVIC_GDO0_InitStruct		NVIC_GDO0_InitStruct

// SD MMC
#define SPI_SDMMC_GPIO_InitStruct		GPIO_SDMMC_SPI_InitStruct
#define SPI_SDMMC_GPIO_CSn_InitStruct		GPIO_SDMMC_CSn_InitStruct
#define SPI_SDMMC_GPIO_CD_InitStruct		GPIO_SDMMC_CD_InitStruct
#define SPI_SDMMC_GPIO_CLK_InitStruct		GPIO_SDMMC_CLK_InitStruct
#define SPI_SDMMC_GPIO_MOSI_InitStruct		GPIO_SDMMC_MOSI_InitStruct
#define SPI_SDMMC_GPIO_MISO_InitStruct		GPIO_SDMMC_MISO_InitStruct
#define SPI_SDMMC_GPIO_CD_InitStruct		GPIO_SDMMC_CD_InitStruct

#define SPI_SDMMC_SPI_InitStruct		SPI_SDMMC_InitStruct

// GSM
#define USART_GSM_GPIO_InitStruct		GPIO_GSM_USART_InitStructure
#define USART_GSM_USART_InitStruct              USART_GSM_InitStruct

// W25Q FLASH
#define SPI_SFLASH_GPIO_InitStruct		GPIO_SFLASH_SPI_InitStruct
#define SPI_SFLASH_GPIO_CSn_InitStruct		GPIO_SFLASH_CSn_InitStruct
#define SPI_SFLASH_GPIO_RES_InitStruct		GPIO_SFLASH_RST_InitStruct
#define SPI_SFLASH_GPIO_DI_InitStruct		GPIO_SFLASH_A0_InitStruct
#define SPI_SFLASH_GPIO_CLK_InitStruct		GPIO_SFLASH_CLK_InitStruct
#define SPI_SFLASH_GPIO_MOSI_InitStruct		GPIO_SFLASH_MOSI_InitStruct
#define SPI_SFLASH_GPIO_MISO_InitStruct		GPIO_SFLASH_MISO_InitStruct
#define SPI_SFLASH_SPI_InitStruct		SPI_SFLASH_InitStruct



/********************NODEBOX868*************************************************/

/* FLAG DECLARATION     	*/
struct Flag_grp
{
  uint8_t			        modeProg;
  uint8_t 			        modeSet;
  uint8_t 			        modeGet;
  uint8_t 			        modeAck;
  uint8_t 			        modeQuit;

  uint8_t 			        cmdNode;
  uint8_t 			        cmdSensor;

  uint8_t 			        param_address;
  uint8_t 			        param_name;
  uint8_t				param_zonename;
  uint8_t				param_zoneaddress;
  uint8_t				param_gpslat;
  uint8_t				param_gpslon;
  uint8_t				param_clock;
  uint8_t				param_type;
  uint8_t				param_onrelay;
  uint8_t				param_offrelay;
  uint8_t				param_tmrinterval;
  uint8_t				param_cmp;
  uint8_t				param_cmpval;
  uint8_t	                        param_flash;

  uint8_t				param_rmin;
  uint8_t				param_rmax;
  uint8_t				param_val;
  uint8_t	                        param_bat;

  uint8_t				cmdStop;
  uint8_t				cmdTyp;

  uint8_t	                        WirelessRx;
};

/* COUNT DECLARATION            */
extern uint8_t                         sensorCnt;

/* TEMPORARY ADDRESS            */
extern byte                             srcAddress;
extern byte                             destAddress;
extern byte                             zoneAddress;

/* REGISTERS DECLARATION        */
struct Node_grp
{
        byte                    id;
        byte 			name[NAME_BYTE];
	byte 			address;
        
        byte                    zone_id;
        uint8_t                 zone_flag[ZONE_PACK];
        byte 			zone_name[ZONE_PACK][NAME_BYTE];
        byte                    zone_address[ZONE_PACK];
        
	byte 			gps_lat[NAME_BYTE];
	byte 			gps_lon[NAME_BYTE];

	uint8_t 		SYS_HR;
	uint8_t 		SYS_MIN;
	uint8_t 		SYS_SEC;
        
        uint8_t 		SYS_DAY;
        uint8_t 		SYS_DD;
	uint8_t 		SYS_MM;
	uint8_t 		SYS_YY;
        
        uint8_t                 TIME_FLAG[RELAY_PACK][ONOFF_PACK];
        
        bool                    DAY_MON[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_TUE[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_WED[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_THUR[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_FRI[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_SAT[RELAY_PACK][ONOFF_PACK];
        bool                    DAY_SUN[RELAY_PACK][ONOFF_PACK];
        
	uint8_t		        ON_HR[RELAY_PACK][ONOFF_PACK];
	uint8_t		        ON_MIN[RELAY_PACK][ONOFF_PACK];
        uint8_t		        ON_SEC[RELAY_PACK][ONOFF_PACK];

	uint8_t		        OFF_HR[RELAY_PACK][ONOFF_PACK];
	uint8_t		        OFF_MIN[RELAY_PACK][ONOFF_PACK];
        uint8_t		        OFF_SEC[RELAY_PACK][ONOFF_PACK];
        
        bool                    RELAY[RELAY_PACK];
        
        uint8_t 		bat_v;
        byte                    model_no[NAME_BYTE];
        byte 			version[NAME_BYTE];
};

struct Sensor_grp
{
        unsigned int            sensor_flag;
        byte                    id;
	byte 			name[NAME_BYTE];
	byte 			address;
        byte                    zone_id;
	byte 			zone_name[NAME_BYTE];
        byte 			zone_address;
	byte 			type;
	unsigned int 		val;
        
        uint8_t                 CMP_FLAG[RELAY_PACK][CMP_PACK];
	byte			CMP[RELAY_PACK][CMP_PACK];
	uint8_t              	CMP_VAL[RELAY_PACK][CMP_PACK];

	uint8_t		        TMR_INTVAL_HR;
	uint8_t		        TMR_INTVAL_MIN;
	uint8_t		        TMR_INTVAL_SEC;
        
	uint8_t 		bat_v;
        byte                    model_no[NAME_BYTE];
        byte 			version[NAME_BYTE];
};

extern Flag_grp         flagVar;
extern Node_grp         nodeVar;
extern Sensor_grp       sensorVar[SENSOR_PACK];

/********************LORA*************************************************/
typedef struct 
{
    char		UsartRxBuffer[MAX_BUFFLEN];
}Lora_var;

extern Lora_var         loraVar;


/********************VARIABLES*************************************************/
// AT COMMAND VARIABLES 
typedef struct
{
  char                  SendCommand[128];
  char                  ReceiveAnswer[10][64];
  uint32_t              SendCommandStartTime;
  uint32_t              ReceiveAnswerExeTime;
  uint16_t              ReceiveAnswerMaxWaiting;  
  uint8_t               FindAnswer;  
}Sim80xAtCommand_t;

// STATUS VARIABLES
typedef struct
{
  uint8_t               RegisterdToNetwork;
  uint8_t               DataTransferMode:1;
  uint8_t               Busy:1;
  uint8_t               Power:1;
  uint8_t               SmsReady:1;  
  uint8_t               CallReady:1;  
 
  uint8_t               BatteryCharging:1;
  uint8_t               BatteryFull:1;
  uint8_t               BatteryPercent;
  float                 BatteryVoltage;
  
  uint8_t               Signal; 
  
}Sim80xStatus_t;

//GSM VARIABLES
typedef struct
{
  uint8_t               HaveNewCall:1;
  uint8_t               MsgReadIsOK:1;  
  uint8_t               MsgSent:1;  

  GsmVoiceStatus_t      GsmVoiceStatus;         
  char                  CallerNumber[16];
  char                  DiallingNumber[16]; 

  uint8_t               MsgTextModeParameterFo;
  uint8_t               MsgTextModeParameterVp;
  uint8_t               MsgTextModeParameterPid;
  uint8_t               MsgTextModeParameterDcs;
  char                  MsgServiceNumber[16];
  char                  MsgSentNumber[16];
  char                  MsgNumber[16];
  char                  MsgDate[8];
  char                  MsgTime[8];
  char                  Msg[_SIM80X_BUFFER_SIZE]; 
  GsmTECharacterSet_t   TeCharacterFormat;
  GsmMsgMemory_t        MsgMemory;
  GsmMsgFormat_t        MsgFormat;
  uint8_t               MsgCapacity;
  uint8_t               MsgUsed;
  uint8_t               HaveNewMsg[10];  
  
}Sim80xGsm_t;

// VARIABLES GPRS HTTP
typedef struct 
{
  uint8_t                 CopyToBuffer;
  GPRSHttpMethod_t        Method;
  uint16_t                ResultCode;
  uint32_t                DataLen;
  uint32_t                TransferStartAddress;
  uint16_t                TransferDataLen;
  char                    Data[256];
  
}GPRSHttpAction_t;

// VARIABLES GPRS LOGIN SETTINGS
typedef struct
{
  uint8_t               MultiConnection:1;
  char                  APN[17];
  char                  APN_UserName[17];
  char                  APN_Password[17];
  char                  LocalIP[17]; 
  GPRSConnection_t      Connection[5];
  GPRSSendData_t        SendStatus[5];
  
  GPRSHttpAction_t      HttpAction;  
  
}GPRS_t;

// VARIABLES - BLUETOOTH CONNECTION SETTINGS 
typedef struct
{
  uint8_t               Visibility:1;
  uint8_t               NeedGetStatus:1;
  BluetoothStatus_t     Status;
  char                  HostName[19];
  char                  HostAddress[19];
  char                  PairingPassword[17];
  uint8_t               ConnectedID;
  char                  ConnectedName[19];
  char                  ConnectedAddress[19];
  BluetoothProfile_t    ConnectedProfile[5];
  BluetoothProfile_t    ConnectingRequestProfile;
  uint16_t              SPPLen;
  char                  SPPBuffer[1024];  
  
}Sim80xBluetooth_t;


// NESTED MAIN STRUCTURE
typedef struct
{
  uint32_t              BufferStartTime;
  uint8_t               BufferExeTime;
  
  uint16_t	        UsartRxIndex;
  uint8_t		UsartRxTemp;
  char                  UsartRxBuffer[_SIM80X_BUFFER_SIZE];
  
  uint32_t	        UsartRxLastTime;
  
  Sim80xWave_t          WaveState;   
  
  char                  IMEI[16];
  uint8_t               RingVol;
  uint8_t               LoadVol;
  uint8_t               MicGainMain;
  uint8_t               MicGainAux;
  uint8_t               MicGainMainHandsfree;
  uint8_t               MicGainAuxHandsfree;
  uint8_t               ToneVol;
  
  uint16_t              EchoHandset_NonlinearProcessing;
  uint16_t              EchoHandfree_NonlinearProcessing;
  uint16_t              EchoHandset_AcousticEchoCancellation;
  uint16_t              EchoHandfree_AcousticEchoCancellation;
  uint16_t              EchoHandset_NoiseReduction;
  uint16_t              EchoHandfree_NoiseReduction;
  uint16_t              EchoHandset_NoiseSuppression;
  uint16_t              EchoHandfree_NoiseSuppression;

  Sim80xAtCommand_t     AtCommand;
  Sim80xStatus_t        Status;
  Sim80xGsm_t           Gsm;

  #if (_SIM80X_USE_BLUETOOTH==1)
    Sim80xBluetooth_t     Bluetooth;
  #endif

  #if (_SIM80X_USE_GPRS==1)
    GPRS_t                GPRS;
  #endif
  
}Sim80x_t;


extern Sim80x_t                         Sim80x;
extern Sim80xAtCommand_t                GSM_ATCOMMANDS;
extern Sim80xStatus_t                   GSM_STATUS;
extern Sim80xGsm_t                      GSM_GSM; 
extern Sim80xBluetooth_t                GSM_SIM80XBLUETOOTH;
extern GPRSHttpAction_t                 GSM_GPRSHTTPACTION;
extern GPRS_t                           GSM_GPRS;

#endif