#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_SIM800C.h"
#include "LIB_SIM800C.h"

#ifdef LIB_SIM800C_H
  
#if(_USE_GSM==1)

HAL_SIM800C                     sim800c;
Sim80x_t                        Sim80x;

//osThreadId    Sim80xTaskHandle;       // RTOS
//osThreadId    Sim80xBuffTaskHandle;   // RTOS


/****************************************************************
*FUNCTION NAME:test
*FUNCTION     :test codes
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::test(void)  
{
  char temp[30];
    
  bufferProcess();
  delay_ms(300);

  sim800c.sendAtCommand("ATE0\r\n", 200, 0);
  delay_ms(300);
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); // clear buffer
  
  
  sim800c.sendAtCommand("AT\r\n", 200, 0);
  delay_ms(300);
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); // clear buffer
  

//  sim800c.sendAtCommand("AT+GSN\r\n", 200, 0);
//  delay_ms(300);
//  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); // clear buffer

  
  //getIMEI(temp);
  //sim800c.writeString("AT\r\n");
    //sim800c.init(osprior);
  //getRingVol();  
}

/****************************************************************
*FUNCTION NAME:PoweronReset
*FUNCTION     :Switch reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_SIM800C::PoweronReset(void)
{
  #if (_SIM80X_USE_POWER_KEY==1)  
    GPIO_WriteBit(GSM_GPIO_PWR,GSM_PIN_PWR,Bit_SET);
  #else
    delay_ms(1000);
  #endif

  sim800c.hardReset(); 
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :sim800c register config //details refer datasheet of ST7586S//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LIB_SIM800C::RegConfigSettings(void)
{
  memset(&Sim80x,0,sizeof(Sim80x));
  memset(Sim80x.UsartRxBuffer,0,_SIM80X_BUFFER_SIZE);
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	
        
  //osThreadDef(Sim80xTask, startSim80xTask, Priority, 0, 256);         //RTOS
  //Sim80xTaskHandle = osThreadCreate(osThread(Sim80xTask), NULL);        //RTOS
  //osThreadDef(Sim80xBuffTask, startSim80xBuffTask, Priority, 0, 256);   //RTOS
  //Sim80xBuffTaskHandle = osThreadCreate(osThread(Sim80xBuffTask), NULL);        //RTOS
        
  for(uint8_t i=0 ;i<10 ;i++)  
  {
    if(sim800c.sendAtCommand("AT\r\n",200,1,"AT\r\r\nOK\r\n") == 1)
      break;
    delay_ms(200);
  }  
  
  setPower(true); 
  setFactoryDefault();

  sim800c.sendAtCommand("ATE1\r\n",200,1,"ATE1\r\r\nOK\r\n");
  sim800c.sendAtCommand("AT+COLP=1\r\n",200,1,"AT+COLP=1\r\r\nOK\r\n");
  sim800c.sendAtCommand("AT+CLIP=1\r\n",200,1,"AT+CLIP=1\r\r\nOK\r\n");
  sim800c.sendAtCommand("AT+FSHEX=0\r\n",200,1,"AT+FSHEX=0\r\r\nOK\r\n");
 // sim800c.sendAtCommand("AT+CREG=1\r\n",200,1,"AT+CREG=1\r\r\nOK\r\n");
  sim800c.sendAtCommand("AT+ECHO?\r\n",200,1,"\r\nOK\r\n");
  Gsm_setMsgMemoryLocation(GsmMsgMemory_OnModule);
  Gsm_setMsgFormat(GsmMsgFormat_Text);
  Gsm_setMsgTextModeParameter(17,167,0,0);
  Gsm_getMsgCharacterFormat();
  getSmsFormat();
  if(Sim80x.Gsm.MsgFormat != GsmMsgFormat_Text)
    Gsm_setMsgFormat(GsmMsgFormat_Text);
  Gsm_getMsgServiceNumber();
  getSmsMsgTextModeParam();
  getIMEI(NULL);
  
#if (_SIM80X_USE_BLUETOOTH==1)
    Bluetooth_setAutoPair(true);
#endif
  sim800c.sendAtCommand("AT+CREG?\r\n",200,1,"\r\n+CREG:");  
  //coreUserInit();
}

/****************************************************************
*FUNCTION NAME:debugTerminal
*FUNCTION     :Prints response to terminal IO
*INPUT        :message
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::debugTerminal(char *msg)
{
  #if (_SIM80X_DEBUG== 1 || _SIM80X_DEBUG==2)
     char      *strStart,*str1;

     strStart = (char*)&Sim80x.UsartRxBuffer[0];  
      str1 = strstr(strStart,"\r\nOK\r\n");

      if(str1!=NULL)
      {
        #if (_SIM80X_DEBUG==2)
          printf("\r\nAT Respone : %s",Sim80x.UsartRxBuffer);
        #endif
        printf("\r\n%s ---> OK\r\n", msg);
      }
      else
        printf("\r\n% ---> FAILED\r\n", msg);

  #endif
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :Initialisation
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::Init(void)  
{
  sim800c.gpioInit();
  sim800c.uartInit();
  PoweronReset();
  RegConfigSettings();
}

/****************************************************************
*FUNCTION NAME:softReset
*FUNCTION     :softReset
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::softReset(void)
{

}

/****************************************************************
*FUNCTION NAME:probe
*FUNCTION     :probe
*INPUT        :timeout
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::probe(unsigned long timeout )
{

}

/****************************************************************
*FUNCTION NAME:RxCallBack
*FUNCTION     :RxCallBack
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void	LIB_SIM800C::rxCallBack(void)
{
  if((Sim80x.Status.DataTransferMode==0)&&(Sim80x.UsartRxTemp!=0))
  {
    //Sim80x.UsartRxLastTime = HAL_GetTick();
    Sim80x.UsartRxBuffer[Sim80x.UsartRxIndex] = Sim80x.UsartRxTemp;
    if(Sim80x.UsartRxIndex < (_SIM80X_BUFFER_SIZE-1))
      Sim80x.UsartRxIndex++;
  }
  else if(Sim80x.Status.DataTransferMode==1)
  {
    //Sim80x.UsartRxLastTime = HAL_GetTick();
    Sim80x.UsartRxBuffer[Sim80x.UsartRxIndex] = Sim80x.UsartRxTemp;
    if(Sim80x.UsartRxIndex < (_SIM80X_BUFFER_SIZE-1))
      Sim80x.UsartRxIndex++;
  }
  USART_ITConfig(GSM_USART_CH, USART_IT_RXNE, ENABLE);	
}

/****************************************************************
*FUNCTION NAME:startSim80xBuffTask
*FUNCTION     :startSim80xBuffTask
*INPUT        :argument;
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::startSim80xBuffTask(void const * argument)
{ 
  while(1)
  {
    //if( ((Sim80x.UsartRxIndex>4) && (HAL_GetTick()-Sim80x.UsartRxLastTime > 50)))
    {
      //Sim80x.BufferStartTime = HAL_GetTick();      
      bufferProcess();      
      //Sim80x.BufferExeTime = HAL_GetTick()-Sim80x.BufferStartTime;
    }
    delay_ms(10);
  }    
}

/****************************************************************
*FUNCTION NAME:BufferProcess
*FUNCTION     :bufferProcess
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void  LIB_SIM800C::bufferProcess(void)
{
  char      *strStart,*str1,*str2,*str3;
  int32_t   tmp_int32_t;
  char      tmp_str[16];
  
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
  //##################################################
  //+++       Buffer Process
  //##################################################
  str1 = strstr(strStart,"\r\n+CREG:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,',');
    str1++;
    if(atoi(str1)==1)
      Sim80x.Status.RegisterdToNetwork=1;
    else
      Sim80x.Status.RegisterdToNetwork=0;
  }
  //##################################################
  str1 = strstr(strStart,"\r\nCall Ready\r\n");
  if(str1!=NULL)
    Sim80x.Status.CallReady=1;  
  //##################################################
  str1 = strstr(strStart,"\r\nSMS Ready\r\n");
  if(str1!=NULL)
    Sim80x.Status.SmsReady=1;  
  //##################################################
  str1 = strstr(strStart,"\r\n+COLP:");
  if(str1!=NULL)
  {
    Sim80x.Gsm.GsmVoiceStatus = GsmVoiceStatus_MyCallAnswerd;
  }  
  //##################################################
  str1 = strstr(strStart,"\r\n+CLIP:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,'"');
    str1++;
    str2 = strchr(str1,'"');
    strncpy(Sim80x.Gsm.CallerNumber,str1,str2-str1);
    Sim80x.Gsm.HaveNewCall=1;  
  }  
  //##################################################
  str1 = strstr(strStart,"\r\n+CSQ:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.Status.Signal = atoi(str1);      
  }
  //##################################################
  str1 = strstr(strStart,"\r\n+CBC:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    tmp_int32_t = atoi(str1);
    if(tmp_int32_t==0)
    {
      Sim80x.Status.BatteryCharging=0;
      Sim80x.Status.BatteryFull=0;
    }
    if(tmp_int32_t==1)
    {
      Sim80x.Status.BatteryCharging=1;
      Sim80x.Status.BatteryFull=0;
    }
    if(tmp_int32_t==2)
    {
      Sim80x.Status.BatteryCharging=0;
      Sim80x.Status.BatteryFull=1;
    }
    str1 = strchr(str1,',');
    str1++;
    Sim80x.Status.BatteryPercent = atoi(str1);
    str1 = strchr(str1,',');
    str1++;
    Sim80x.Status.BatteryVoltage = atof(str1)/1000;      
  }
  //##################################################
  str1 = strstr(strStart,"\r\nBUSY\r\n");
  if(str1!=NULL)
  {
    Sim80x.Gsm.GsmVoiceStatus=GsmVoiceStatus_ReturnBusy;
  }
  //##################################################
  str1 = strstr(strStart,"\r\nNO DIALTONE\r\n");
  if(str1!=NULL)
  {
    Sim80x.Gsm.GsmVoiceStatus=GsmVoiceStatus_ReturnNoDialTone;
  }
  //##################################################
  str1 = strstr(strStart,"\r\nNO CARRIER\r\n");
  if(str1!=NULL)
  {
    Sim80x.Gsm.GsmVoiceStatus=GsmVoiceStatus_ReturnNoCarrier;
  }
  //##################################################
  str1 = strstr(strStart,"\r\nNO ANSWER\r\n");
  if(str1!=NULL)
  {
    Sim80x.Gsm.GsmVoiceStatus=GsmVoiceStatus_ReturnNoAnswer;
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+CMGS:");
  if(str1!=NULL)
  {
    Sim80x.Gsm.MsgSent=1;
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+CPMS:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    str1++;
    if(*str1 == '"')
    {
      str1 = strchr(str1,',');
      str1++;
    }
    Sim80x.Gsm.MsgUsed = atoi(str1);
    str1 = strchr(str1,',');
    str1++;      
    Sim80x.Gsm.MsgCapacity = atoi(str1);
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+CMGR:");
  if(str1!=NULL)
  {
    if(Sim80x.Gsm.MsgFormat == GsmMsgFormat_Text)
    {
      memset(Sim80x.Gsm.Msg,0,sizeof(Sim80x.Gsm.Msg));
      memset(Sim80x.Gsm.MsgDate,0,sizeof(Sim80x.Gsm.MsgDate));
      memset(Sim80x.Gsm.MsgNumber,0,sizeof(Sim80x.Gsm.MsgNumber));
      memset(Sim80x.Gsm.MsgTime,0,sizeof(Sim80x.Gsm.MsgTime));
      tmp_int32_t = sscanf(str1,"\r\n+CMGR: %*[^,],\"%[^\"]\",%*[^,],\"%[^,],%[^+-]%*d\"\r\n%[^\r]s\r\nOK\r\n",Sim80x.Gsm.MsgNumber,Sim80x.Gsm.MsgDate,Sim80x.Gsm.MsgTime,Sim80x.Gsm.Msg);      
      if(tmp_int32_t == 4)
        Sim80x.Gsm.MsgReadIsOK=1;
      else
        Sim80x.Gsm.MsgReadIsOK=0;
    }else if(Sim80x.Gsm.MsgFormat == GsmMsgFormat_PDU)
    {

      
    }    
  }
  //################################################## 
  str1 = strstr(strStart,"\r\n+CRSL:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.RingVol = atoi(str1);    
  }
  //################################################## 
  str1 = strstr(strStart,"\r\n+CLVL:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.LoadVol = atoi(str1);    
  }
  //################################################## 
  str1 = strstr(strStart,"\r\n+CMTI:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,',');
    str1++;
    for(uint8_t i=0 ;i<sizeof(Sim80x.Gsm.HaveNewMsg) ; i++)
    {
      if(Sim80x.Gsm.HaveNewMsg[i]==0)
      {
        Sim80x.Gsm.HaveNewMsg[i] = atoi(str1);    
        break;
      }
    }
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+CSCA:");
  if(str1!=NULL)
  {
    memset(Sim80x.Gsm.MsgServiceNumber,0,sizeof(Sim80x.Gsm.MsgServiceNumber));
    str1 = strchr(str1,'"');
    str1++;
    str2 = strchr(str1,'"');
    strncpy(Sim80x.Gsm.MsgServiceNumber,str1,str2-str1);
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+CSMP:");
  if(str1!=NULL)
  {
    tmp_int32_t = sscanf(str1,"\r\n+CSMP: %hhd,%hhd,%hhd,%hhd\r\nOK\r\n",&Sim80x.Gsm.MsgTextModeParameterFo,&Sim80x.Gsm.MsgTextModeParameterVp,&Sim80x.Gsm.MsgTextModeParameterPid,&Sim80x.Gsm.MsgTextModeParameterDcs);
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+CUSD:");
  if(str1!=NULL)
  {
    sscanf(str1,"\r\n+CUSD: 0, \"%[^\r]s",Sim80x.Gsm.Msg);    
    tmp_int32_t = strlen(Sim80x.Gsm.Msg);
    if(tmp_int32_t > 5)
    {
      Sim80x.Gsm.Msg[tmp_int32_t-5] = 0;
    }
  }

  //##################################################  
  str1 = strstr(strStart,"\r\n+CREC: ");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.WaveState = (Sim80xWave_t)atoi(str1);
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+CMIC: ");
  if(str1!=NULL)
  {
    while(strchr(str1,'(')!=NULL)
    {
      str1 = strchr(str1,'(');
      str1++;
      tmp_int32_t = atoi(str1);
      switch(tmp_int32_t)
      {
        case 0:
          str1 = strchr(str1,',');
          str1++;
          Sim80x.MicGainMain = atoi(str1);
        break;        
        case 1:
          str1 = strchr(str1,',');
          str1++;
          Sim80x.MicGainAux = atoi(str1);
        break;        
        case 2:
          str1 = strchr(str1,',');
          str1++;
          Sim80x.MicGainMainHandsfree = atoi(str1);
        break;        
        case 3:
          str1 = strchr(str1,',');
          str1++;
          Sim80x.MicGainAuxHandsfree = atoi(str1);
        break;        
      }
    }    
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+SNDLEVEL:");
  if(str1!=NULL)
  {
    while(strchr(str1,'(')!=NULL)
    {
      str1 = strchr(str1,'(');
      str1++;
      tmp_int32_t = atoi(str1);
      switch(tmp_int32_t)
      {
        case 0:
          str1 = strchr(str1,',');
          str1++;
          Sim80x.ToneVol = atoi(str1);
        break;        
        case 1:
          str1 = strchr(str1,',');
          str1++;
          // ...        
        break;        
      }
    }
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+ECHO: ");
  if(str1!=NULL)
  {
    sscanf(str1,"\r\n+ECHO: (0,%hu,%hu,%hu,%hu),(1,%hu,%hu,%hu,%hu)",&Sim80x.EchoHandset_NonlinearProcessing,&Sim80x.EchoHandset_AcousticEchoCancellation,&Sim80x.EchoHandset_NoiseReduction,&Sim80x.EchoHandset_NoiseSuppression,\
    &Sim80x.EchoHandfree_NonlinearProcessing,&Sim80x.EchoHandfree_AcousticEchoCancellation,&Sim80x.EchoHandfree_NoiseReduction,&Sim80x.EchoHandfree_NoiseSuppression);
  }  
  //##################################################  
  
  //##################################################  
  
  //##################################################  
  
  //##################################################  
  //##################################################   
  //################################################## 
  #if( _SIM80X_USE_BLUETOOTH==1)  
  //################################################## 
  str1 = strstr(strStart,"\r\n+BTHOST:");
  if(str1!=NULL)
  {
    sscanf(str1,"\r\n+BTHOST: %[^,],%[^\r]",Sim80x.Bluetooth.HostName,Sim80x.Bluetooth.HostAddress);    
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTSTATUS:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.Bluetooth.Status = (BluetoothStatus_t)atoi(str1);
    
    str3 = strstr(str1,"\r\nOK\r\n");
    CheckAnotherConnectedProfile:
    str2 = strstr(str1,"\r\nC:");
    if((str2 != NULL) && (str2 <str3) && (str2 > str1))
    {
      tmp_int32_t = sscanf(str2,"\r\nC: %hhd,%[^,],%[^,],%[^\r]",&Sim80x.Bluetooth.ConnectedID,Sim80x.Bluetooth.ConnectedName,Sim80x.Bluetooth.ConnectedAddress,tmp_str);       
      if(strcmp(tmp_str,"\"HFP\"")==0)
        tmp_int32_t = BluetoothProfile_HSP_HFP;
      else if(strcmp(tmp_str,"\"HSP\"")==0)
        tmp_int32_t = BluetoothProfile_HSP_HFP;
      else if(strcmp(tmp_str,"\"A2DP\"")==0)
        tmp_int32_t = BluetoothProfile_A2DP;
      else if(strcmp(tmp_str,"\"GAP\"")==0)
        tmp_int32_t = BluetoothProfile_GAP;
      else if(strcmp(tmp_str,"\"GOEP\"")==0)
        tmp_int32_t = BluetoothProfile_GOEP;
      else if(strcmp(tmp_str,"\"OPP\"")==0)
        tmp_int32_t = BluetoothProfile_OPP;
      else if(strcmp(tmp_str,"\"SDAP\"")==0)
        tmp_int32_t = BluetoothProfile_SDAP;
      else if(strcmp(tmp_str,"\"SPP\"")==0)
        tmp_int32_t = BluetoothProfile_SSP;
      else tmp_int32_t = BluetoothProfile_NotSet;
    
      for(uint8_t i=0 ;i<sizeof(Sim80x.Bluetooth.ConnectedProfile) ; i++)
      {
        if(Sim80x.Bluetooth.ConnectedProfile[i]==(BluetoothProfile_t)tmp_int32_t)
          break;
        if(Sim80x.Bluetooth.ConnectedProfile[i]==BluetoothProfile_NotSet)
        {
          Sim80x.Bluetooth.ConnectedProfile[i]=(BluetoothProfile_t)tmp_int32_t;
          break;
        }
      }
      str1+=5;
      goto CheckAnotherConnectedProfile;
    }    
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTPAIRING:");
  if(str1!=NULL)
  {
    Sim80x.Bluetooth.ConnectedID=0;
    memset(Sim80x.Bluetooth.ConnectedAddress,0,sizeof(Sim80x.Bluetooth.ConnectedAddress));
    memset(Sim80x.Bluetooth.ConnectedName,0,sizeof(Sim80x.Bluetooth.ConnectedName));
    tmp_int32_t = sscanf(str1,"\r\n+BTPAIRING: \"%[^\"]\",%[^,],%[^\r]",Sim80x.Bluetooth.ConnectedName,Sim80x.Bluetooth.ConnectedAddress,Sim80x.Bluetooth.PairingPassword);
    if(tmp_int32_t == 3)
      Sim80x.Bluetooth.ConnectedID = 255;      
  }    
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTPAIR:");
  if(str1!=NULL)
  {
    
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTCONNECT:");
  if(str1!=NULL)
  {
    Sim80x.Bluetooth.ConnectedID=0;
    memset(Sim80x.Bluetooth.ConnectedAddress,0,sizeof(Sim80x.Bluetooth.ConnectedAddress));
    memset(Sim80x.Bluetooth.ConnectedName,0,sizeof(Sim80x.Bluetooth.ConnectedName));
    tmp_int32_t = sscanf(str1,"\r\n+BTCONNECT: %hhd,\"%[^\"]\",%[^,],%[^\r]",&Sim80x.Bluetooth.ConnectedID,Sim80x.Bluetooth.ConnectedName,Sim80x.Bluetooth.ConnectedAddress,tmp_str);
    if(strcmp(tmp_str,"\"HFP\"")==0)
      tmp_int32_t = BluetoothProfile_HSP_HFP;
    else if(strcmp(tmp_str,"\"HSP\"")==0)
      tmp_int32_t = BluetoothProfile_HSP_HFP;
    else if(strcmp(tmp_str,"\"A2DP\"")==0)
      tmp_int32_t = BluetoothProfile_A2DP;
    else if(strcmp(tmp_str,"\"GAP\"")==0)
      tmp_int32_t = BluetoothProfile_GAP;
    else if(strcmp(tmp_str,"\"GOEP\"")==0)
      tmp_int32_t = BluetoothProfile_GOEP;
    else if(strcmp(tmp_str,"\"OPP\"")==0)
      tmp_int32_t = BluetoothProfile_OPP;
    else if(strcmp(tmp_str,"\"SDAP\"")==0)
      tmp_int32_t = BluetoothProfile_SDAP;
    else if(strcmp(tmp_str,"\"SPP\"")==0)
      tmp_int32_t = BluetoothProfile_SSP;
    else tmp_int32_t = BluetoothProfile_NotSet;
    
    for(uint8_t i=0 ;i<sizeof(Sim80x.Bluetooth.ConnectedProfile) ; i++)
    {
      if(Sim80x.Bluetooth.ConnectedProfile[i]==(BluetoothProfile_t)tmp_int32_t)
        break;
      if(Sim80x.Bluetooth.ConnectedProfile[i]==BluetoothProfile_NotSet)
      {
        Sim80x.Bluetooth.ConnectedProfile[i]=(BluetoothProfile_t)tmp_int32_t;
        break;
      }
    }
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTCONNECTING:");
  if(str1!=NULL)
  {
    memset(tmp_str,0,sizeof(tmp_str));
    str1 = strchr(str1,',');
    str1++;
    str2 = strchr(str1+1,'"');
    str2++;
    strncpy(tmp_str,str1,str2-str1);
    if(strcmp(tmp_str,"\"HFP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_HSP_HFP;
    else if(strcmp(tmp_str,"\"HSP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_HSP_HFP;
    else if(strcmp(tmp_str,"\"A2DP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_A2DP;
    else if(strcmp(tmp_str,"\"GAP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_GAP;
    else if(strcmp(tmp_str,"\"GOEP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_GOEP;
    else if(strcmp(tmp_str,"\"OPP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_OPP;
    else if(strcmp(tmp_str,"\"SDAP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_SDAP;
    else if(strcmp(tmp_str,"\"SPP\"")==0)
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_SSP;
    else Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_NotSet;
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTDISCONN:");
  if(str1!=NULL)
  {
    Sim80x.Bluetooth.ConnectedID=0;    
    memset(Sim80x.Bluetooth.ConnectedProfile,0,sizeof(Sim80x.Bluetooth.ConnectedProfile));
    memset(Sim80x.Bluetooth.ConnectedAddress,0,sizeof(Sim80x.Bluetooth.ConnectedAddress));
    memset(Sim80x.Bluetooth.ConnectedName,0,sizeof(Sim80x.Bluetooth.ConnectedName));    
    Sim80x.Bluetooth.NeedGetStatus=1;
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTVIS:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.Bluetooth.Visibility=atoi(str1);
  }
  //##################################################  
  str1 = strstr(strStart,"\r\n+BTSPPDATA:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,',');
    str1++;
    tmp_int32_t = atoi(str1);
    str1 = strchr(str1,',');
    str1++;
    strncpy(Sim80x.Bluetooth.SPPBuffer,str1,tmp_int32_t);    
    Sim80x.Bluetooth.SPPLen = tmp_int32_t;
  }  
  //##################################################  
  
  //##################################################  
  
  //##################################################  
  
  //##################################################  
  
  
  //##################################################  
  #endif
  //##################################################  
  //##################################################  
  //##################################################
  #if (_SIM80X_USE_GPRS==1)
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGDCONT:");
  if(str1!=NULL)
  {
    
  }  
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGQMIN:");
  if(str1!=NULL)
  {
    
  }    
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGQREQ:");
  if(str1!=NULL)
  {
    
  }    
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGACT:");
  if(str1!=NULL)
  {
    
  }   
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGPADDR:");
  if(str1!=NULL)
  {
    
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGCLASS:");
  if(str1!=NULL)
  {
    
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGEREP:");
  if(str1!=NULL)
  {
    
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+CGREG:");
  if(str1!=NULL)
  {
    
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+CSTT:");
  if(str1!=NULL)
  {
    sscanf(str1,"\r\n+CSTT: \"%[^\"]\",\"%[^\"]\",\"%[^\"]\"\r\n",Sim80x.GPRS.APN,Sim80x.GPRS.APN_UserName,Sim80x.GPRS.APN_Password);    
  }
  //##################################################  
  str1 = strstr(strStart,"AT+CIFSR\r\r\n");
  if(str1!=NULL)
  {
    sscanf(str1,"AT+CIFSR\r\r\n%[^\r]",Sim80x.GPRS.LocalIP);
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+CIPMUX:");
  if(str1!=NULL)
  {
    str1 =strchr(str1,':');
    str1++;
    if(atoi(str1)==0)
      Sim80x.GPRS.MultiConnection=0;
    else
      Sim80x.GPRS.MultiConnection=1;
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\nCONNECT OK\r\n");
  if(str1!=NULL)
  {
    if(Sim80x.GPRS.MultiConnection==0)
    {
      Sim80x.GPRS.Connection[0] = GPRSConnection_ConnectOK;      
    }
    else
    {

    }   
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\nCONNECT FAIL\r\n");
  if(str1!=NULL)
  {
    if(Sim80x.GPRS.MultiConnection==0)
    {
      Sim80x.GPRS.Connection[0] = GPRSConnection_ConnectFail;      
    }
    else
    {

    }   
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\nALREADY CONNECT\r\n");
  if(str1!=NULL)
  {
    if(Sim80x.GPRS.MultiConnection==0)
    {
      Sim80x.GPRS.Connection[0] = GPRSConnection_AlreadyConnect;      
    }
    else
    {

    }   
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\nSEND OK\r\n");
  if(str1!=NULL)
  {
    if(Sim80x.GPRS.MultiConnection==0)
    {
      Sim80x.GPRS.SendStatus[0] = GPRSSendData_SendOK;      
    }
    else
    {

    }   
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+HTTPACTION:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.GPRS.HttpAction.Method=(GPRSHttpMethod_t)atoi(str1);
    str1 = strchr(str1,',');
    str1++;
    Sim80x.GPRS.HttpAction.ResultCode = atoi(str1);
    str1 = strchr(str1,',');
    str1++;
    Sim80x.GPRS.HttpAction.DataLen = atoi(str1);    
  } 
  //##################################################  
  str1 = strstr(strStart,"\r\n+HTTPREAD:");
  if(str1!=NULL)
  {
    str1 = strchr(str1,':');
    str1++;
    Sim80x.GPRS.HttpAction.TransferDataLen = atoi(str1);
    str1 = strchr(str1,'\n');
    str1++;
    strncpy(Sim80x.GPRS.HttpAction.Data,str1,Sim80x.GPRS.HttpAction.TransferDataLen);
    Sim80x.GPRS.HttpAction.CopyToBuffer=1;
  }
  //##################################################    
  #endif  
  //##################################################  
  //##################################################  
  //##################################################  
  for( uint8_t parameter=0; parameter<11; parameter++)
  {
    if((parameter==10) || (Sim80x.AtCommand.ReceiveAnswer[parameter][0]==0))
    {
      Sim80x.AtCommand.FindAnswer=0;
      break;
    }
    str1 = strstr(strStart,Sim80x.AtCommand.ReceiveAnswer[parameter]);
    if(str1!=NULL)
    {
      Sim80x.AtCommand.FindAnswer = parameter+1;
      //Sim80x.AtCommand.ReceiveAnswerExeTime = HAL_GetTick()-Sim80x.AtCommand.SendCommandStartTime;
      break;
    }    
  }
  //##################################################
  //---       Buffer Process
  //##################################################
  #if (_SIM80X_DEBUG==2)
  printf("%s",strStart);
  #endif
  Sim80x.UsartRxIndex=0;
  memset(Sim80x.UsartRxBuffer,0,_SIM80X_BUFFER_SIZE);    
  Sim80x.Status.Busy=0;
}

/****************************************************************
*FUNCTION NAME:startSim80xTask
*FUNCTION     :startSim80xTask
*INPUT        :argument; 
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::startSim80xTask(void const * argument)
{ 
  uint32_t TimeForSlowRun=0;
  #if( _SIM80X_USE_GPRS==1)
  uint32_t TimeForSlowRunGPRS=0;
  #endif
  uint8_t UnreadMsgCounter=1;
  while(1)
  {    
    //###########################################
    #if( _SIM80X_USE_BLUETOOTH==1)
    //###########################################
    if(Sim80x.Bluetooth.SPPLen >0 )
    {      
      Bluetooth_userNewSppData(Sim80x.Bluetooth.SPPBuffer,Sim80x.Bluetooth.SPPLen);
      Sim80x.Bluetooth.SPPLen=0;
    }
    //###########################################
    if(Sim80x.Bluetooth.NeedGetStatus==1)
    {
      Sim80x.Bluetooth.NeedGetStatus=0;
      Bluetooth_getStatus();
    }    
    //###########################################
    if(Sim80x.Bluetooth.ConnectingRequestProfile != BluetoothProfile_NotSet)
    {
      Bluetooth_userConnectingSpp();
      Sim80x.Bluetooth.ConnectingRequestProfile = BluetoothProfile_NotSet;          
    }
    //###########################################
    if(Sim80x.Bluetooth.ConnectedID==255)
    {
      Sim80x.Bluetooth.ConnectedID=0;
      Bluetooth_userNewPairingRequest(Sim80x.Bluetooth.ConnectedName,Sim80x.Bluetooth.ConnectedAddress,Sim80x.Bluetooth.PairingPassword);      
    }
    //###########################################
    #endif
    //###########################################
    //###########################################
    #if( _SIM80X_USE_GPRS==1)
    //###########################################
    if(HAL_GetTick()-TimeForSlowRunGPRS > 5000)
    {
      
      //TimeForSlowRunGPRS=HAL_GetTick();
    }
    
    
    

    //###########################################
    #endif
    //###########################################
    for(uint8_t i=0 ;i<sizeof(Sim80x.Gsm.HaveNewMsg) ; i++)
    {
      if(Sim80x.Gsm.HaveNewMsg[i] > 0)
      {
        //getSmsMsgMemorySts();        
        if(getSmsMsg(Sim80x.Gsm.HaveNewMsg[i])==true)
        {
					delay_ms(100);
          Gsm_userNewMsg(Sim80x.Gsm.MsgNumber,Sim80x.Gsm.MsgDate,Sim80x.Gsm.MsgTime,Sim80x.Gsm.Msg);
					delay_ms(100);
          deleteSmsMsg(Sim80x.Gsm.HaveNewMsg[i]);
					delay_ms(100);
        }
        getSmsMsgMemorySts();  
        Sim80x.Gsm.HaveNewMsg[i]=0;
      }        
    }    
    //###########################################
    if(Sim80x.Gsm.MsgUsed > 0)
    {   
      if(getSmsMsg(UnreadMsgCounter)==true)
      {
        Gsm_userNewMsg(Sim80x.Gsm.MsgNumber,Sim80x.Gsm.MsgDate,Sim80x.Gsm.MsgTime,Sim80x.Gsm.Msg);
        deleteSmsMsg(UnreadMsgCounter);
        getSmsMsgMemorySts();
      }
      UnreadMsgCounter++;
      if(UnreadMsgCounter==150)
        UnreadMsgCounter=0;      
    }
    //###########################################
    if(Sim80x.Gsm.HaveNewCall == 1)
    {
      Sim80x.Gsm.GsmVoiceStatus = GsmVoiceStatus_Ringing;
      Sim80x.Gsm.HaveNewCall = 0;
     }    
    //###########################################
    //if(HAL_GetTick() - TimeForSlowRun > 20000)
    {
      sim800c.sendAtCommand("AT+CSQ\r\n",200,1,"\r\n+CSQ:");  
      sim800c.sendAtCommand("AT+CBC\r\n",200,1,"\r\n+CBC:"); 
      sim800c.sendAtCommand("AT+CREG?\r\n",200,1,"\r\n+CREG:");  
      getSmsMsgMemorySts();      
      //TimeForSlowRun=HAL_GetTick();
    }
    //###########################################
    //Gsm_User(HAL_GetTick());
    //###########################################
    delay_ms(100);
    
  }    
}

/****************************************************************
*FUNCTION NAME:setAtMode
*FUNCTION     :setAtMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setAtMode(void)
{
  sim800c.sendAtCommand("+++\r\n",200, 0);
  debugTerminal("setAtMode");
}

/****************************************************************
*FUNCTION NAME:setDataMode
*FUNCTION     :setDataMode
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setDataMode(void)
{
  sim800c.sendAtCommand("ATO\r\n",200, 0);
  debugTerminal("setDataMode");
}

/****************************************************************
*FUNCTION NAME:setActiveProfile
*FUNCTION     :setActiveProfile
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setActiveProfile(void)
{
}

/****************************************************************
*FUNCTION NAME:setDefaultConfig
*FUNCTION     :setDefaultConfig
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setDefaultConfig(void)
{
  sim800c.sendAtCommand("ATZ\r\r",200, 0);

  debugTerminal("setDefaultConfig");
}

/****************************************************************
*FUNCTION NAME:setFactoryDefault
*FUNCTION     :setFactoryDefault
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setFactoryDefault(void)
{
  sim800c.sendAtCommand("AT&F0\r\n",200,0);

  debugTerminal("Sim80x_setFactoryDefault");

  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

}

/****************************************************************
*FUNCTION NAME:setAtEcho
*FUNCTION     :change command echo mode
*INPUT        :turnon
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setAtEcho(bool turnon)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "ATE%d\r\n", turnon); 
  sim800c.sendAtCommand(temp, 200, 0);

  debugTerminal("Sim80x_setAtEcho");//need to check as response changes can be on or off

  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

}

/****************************************************************
*FUNCTION NAME:setBaudRate
*FUNCTION     :AT command to change baud rate
*INPUT        :baudrate
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setBaudRate(uint16_t baudrate)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+IPR=%d\r\n", baudrate); 
  sim800c.sendAtCommand(temp, 200, 0);

  debugTerminal("Sim80x_setBaudRate");

  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 
}

/****************************************************************
*FUNCTION NAME:setPower
*FUNCTION     :setPower
*INPUT        :turnon
*OUTPUT       :void
****************************************************************/
void  LIB_SIM800C::setPower(bool turnon)
{ 
  if(turnon==true)
  {  
    if(sim800c.sendAtCommand("AT\r\n",200,1,"AT\r\r\nOK\r\n") == 1)
    {
      delay_ms(100);
      #if (_SIM80X_DEBUG==1)
      printf("\r\nSim80x_SetPower(ON) ---> OK\r\n");
      #endif
      Sim80x.Status.Power=1;
      //initValue();
    }
    else
    {     
      #if (_SIM80X_USE_POWER_KEY==1)  
      GPIO_WriteBit(GSM_GPIO_PWR,GSM_PIN_PWR,Bit_RESET);
      delay_ms(1200);
      GPIO_WriteBit(GSM_GPIO_PWR,GSM_PIN_PWR,Bit_SET);
      #endif
      delay_ms(3000);  
      if(sim800c.sendAtCommand("AT\r\n",200,1,"AT\r\r\nOK\r\n") == 1)
      {
        delay_ms(10000);
        #if (_SIM80X_DEBUG==1)
        printf("\r\nSim80x_SetPower(ON) ---> OK\r\n");
        #endif
        Sim80x.Status.Power=1;
        //initValue();   
      }
      else
        Sim80x.Status.Power=0;
    }
  }
  else
  {
    if(sim800c.sendAtCommand("AT\r\n",200,1,"AT\r\r\nOK\r\n") == 1)
    {
      #if (_SIM80X_DEBUG==1)
      printf("\r\nSim80x_SetPower(OFF) ---> OK\r\n");
      #endif
      Sim80x.Status.Power=0;
      #if (_SIM80X_USE_POWER_KEY==1) 
      GPIO_WriteBit(GSM_GPIO_PWR,GSM_PIN_PWR,Bit_RESET);
      delay_ms(1200);
      GPIO_WriteBit(GSM_GPIO_PWR,GSM_PIN_PWR,Bit_SET);
      #endif
      #if (_SIM80X_USE_POWER_KEY==0)
      sim800c.sendAtCommand("AT+CPOWD=1\r\n",2000,1,"\r\nOK\r\n"); 
      #endif      
    }
  }  
}

/****************************************************************
*FUNCTION NAME:saveParameters
*FUNCTION     :saveParameters
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void   LIB_SIM800C::setParameters(void)
{
  sim800c.sendAtCommand("AT&W0\r\n",200,1,"AT&W0\r\r\nOK\r\n");
  debugTerminal("setParameters()");

  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
}

/****************************************************************
*FUNCTION NAME:getManNo
*FUNCTION     :getManNo 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getManNo(void)
{
  sim800c.sendAtCommand("AT+CGMI\r\n",200,0);

  debugTerminal("Sim80x_getManNo");

  //update
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

  
}

/****************************************************************
*FUNCTION NAME:getModelNo
*FUNCTION     :getModelNo 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getModelNo(void)
{
  sim800c.sendAtCommand("AT+CGMM\r\n",200,0);

  debugTerminal("Sim80x_getModelNo");

  //update
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

  
}

/****************************************************************
*FUNCTION NAME:getGlobalNo
*FUNCTION     :getGlobalNo 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getGlobalNo(void)
{
  sim800c.sendAtCommand("AT+GOI\r\n",200,0);

  debugTerminal("Sim80x_getGlobalNo");

  //update
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

  
}

/****************************************************************
*FUNCTION NAME:getLastCommand
*FUNCTION     :getLastCommand 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getLastCommand(void)
{
   sim800c.sendAtCommand("A/",200, 0);
   //RESPONSE CANT BE DETERMINED
}

/****************************************************************
*FUNCTION NAME:getCurrentConfig
*FUNCTION     :getCurrentConfig 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getCurrentConfig(void)
{
  sim800c.sendAtCommand("AT&V0\r\n",200,0);

  debugTerminal("Sim80x_setFactoryDefault");

  //update
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

  
}

/****************************************************************
*FUNCTION NAME:getTaCapabilities
*FUNCTION     :getTaCapabilities 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getTaCapabilities(void)
{
  sim800c.sendAtCommand("AT+GCAP\r\n",200,0);

  debugTerminal("Sim80x_getTaCapabilities");

  //update
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer)); 

}

/****************************************************************
*FUNCTION NAME:getIMEI
*FUNCTION     :recieve module IMEI number 
*INPUT        :IMEI
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getIMEI(char *IMEI)
{


  sim800c.sendAtCommand("AT+CGSN\r\n",200, 0);

  updateIMEI(1);
}

/****************************************************************
*FUNCTION NAME:getTeChar
*FUNCTION     :getTeChar 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getTeChar(void)
{


  sim800c.sendAtCommand("AT+CSCS?\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getIMEI
*FUNCTION     :recieve module IMEI number 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getAddrsType(void)
{


  sim800c.sendAtCommand("AT+CSTA?\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getIMSI
*FUNCTION     :getIMSI 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getIMSI(void)
{


  sim800c.sendAtCommand("AT+CIMI\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getSelOperator
*FUNCTION     :getSelOperator
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getSelOperator(void)
{


  sim800c.sendAtCommand("AT+COPS?\r\n",120000, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getNetworkReg
*FUNCTION     :getNetworkReg 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getNetworkReg(void)
{


  sim800c.sendAtCommand("AT+CREG?\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getRLPParams
*FUNCTION     :getRLPParams
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool LIB_SIM800C::getRLPParams(void)
{
  sim800c.sendAtCommand("AT+CRLP?\r\n",200, 0);
  debugTerminal("setRLPParams");
}

/****************************************************************
*FUNCTION NAME:getCSQ
*FUNCTION     :getCSQ 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getCSQ(void)
{


  sim800c.sendAtCommand("AT+CSQ\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getPrefOperLst
*FUNCTION     :getPrefOperLst 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getPrefOperLst(void)
{


  sim800c.sendAtCommand("AT+CPOL?\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getOperName
*FUNCTION     :getOperName 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getOperName(void)
{


  sim800c.sendAtCommand("AT+COPN\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getPhoneFunc
*FUNCTION     :getPhoneFunc 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getPhoneFunc(void)
{


  sim800c.sendAtCommand("AT+CFUN?\r\n",10000, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:getClockData
*FUNCTION     :getClockData
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool LIB_SIM800C::getClockData(void)
{
  sim800c.sendAtCommand("AT+CCLK?\r\n",200, 0);
  debugTerminal("setClockData");
}

/****************************************************************
*FUNCTION NAME:getBattChar
*FUNCTION     :getBattChar 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool  LIB_SIM800C::getBattChar(void)
{


  sim800c.sendAtCommand("AT+CBC\r\n",200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:setTeChar
*FUNCTION     :setTeChar
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setTeChar(char *chset)
{
   char temp[20];
   snprintf(temp, sizeof(temp), "AT+CSCS%s\r\n", chset); 
   sim800c.sendAtCommand(temp, 200, 0);
   debugTerminal("setTeChar");
}

/****************************************************************
*FUNCTION NAME:setAddrsType
*FUNCTION     :setAddrsType
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setAddrsType(uint8_t type)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CSTA=%d\r\n", type); 
  sim800c.sendAtCommand(temp,200, 0);
  debugTerminal("setAddrsType");
}

/****************************************************************
*FUNCTION NAME:setSelOperator
*FUNCTION     :setSelOperator
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setSelOperator(uint8_t mode, uint8_t format, uint8_t oper)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+COPS=%d,[%d[],%d]]\r\n", mode, format, oper); 
  sim800c.sendAtCommand(temp,120000, 0);
  debugTerminal("setSelOperator");
}

/****************************************************************
*FUNCTION NAME:setNetworkReg
*FUNCTION     :setNetworkReg
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setNetworkReg(bool turnon)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CREG=%d\r\n", turnon); 
  sim800c.sendAtCommand(temp,200, 0);
  debugTerminal("setNetworkReg");
}

/****************************************************************
*FUNCTION NAME:setRLPParams
*FUNCTION     :setRLPParams 
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void  LIB_SIM800C::setRLPParams(uint16_t iws,uint16_t mws,uint16_t t2,uint16_t n2,uint16_t t4)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CRLP=%d[,%d[,%d[,%d[,%d]]]]\r\n", iws, mws, t2, n2, t4); 


  sim800c.sendAtCommand(temp,200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:setPrefOperLst
*FUNCTION     :setPrefOperLst
*INPUT        :void
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setPrefOperLst(uint8_t index, uint8_t  format, char *oper)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CPOL=%d[,%d,%s]\r\n", index, format, oper); 
  sim800c.sendAtCommand(temp,200, 0);
  debugTerminal("setPrefOperLst");
}

/****************************************************************
*FUNCTION NAME:setPhoneFunc
*FUNCTION     :setPhoneFunc
*INPUT        :fun, rst
*OUTPUT       :void
****************************************************************/
void LIB_SIM800C::setPhoneFunc(uint8_t fun, uint8_t  rst)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CFUN=%d[,%d]\r\n", fun, rst); 
  sim800c.sendAtCommand(temp,10000, 0);
  debugTerminal("setPhoneFunc");
}

/****************************************************************
*FUNCTION NAME:setClockData
*FUNCTION     :setClockData 
*INPUT        :void
*OUTPUT       :time
****************************************************************/
void  LIB_SIM800C::setClockData(char time)
{
  char temp[40];
  snprintf(temp, sizeof(temp), "AT+CCLK=%s\r\n", time); 
  sim800c.sendAtCommand(temp,200, 0);

  //update
}

/****************************************************************
*FUNCTION NAME:setSimAccess
*FUNCTION     :setSimAccess
*INPUT        :void
*OUTPUT       :length, command
****************************************************************/
void LIB_SIM800C::setSimAccess(uint16_t length, char command)
{
  char temp[20];
  snprintf(temp, sizeof(temp), "AT+CSIM=%d,%s\r\n", length, command); 
  sim800c.sendAtCommand(temp,200, 0);
  debugTerminal("setSimAccess");
}



/****************************************************************
*FUNCTION NAME:updateManNo
*FUNCTION     :updateManNo
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateManNo(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT+CGMI\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT+CGMI\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getManNo");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}

/****************************************************************
*FUNCTION NAME:updateModelNo
*FUNCTION     :updateModelNo
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateModelNo(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT+CGMM\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT+CGMM\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getModelNo");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}//complete

/****************************************************************
*FUNCTION NAME:updateGlobalNo
*FUNCTION     :updateGlobalNo
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateGlobalNo(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT+GOI\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT+GOI\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getGlobalNo");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}

/****************************************************************
*FUNCTION NAME:updateLastCommand
*FUNCTION     :updateLastCommand
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateLastCommand(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"A/\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"A/\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getLastCommand");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}

/****************************************************************
*FUNCTION NAME:updateCurrentConfig
*FUNCTION     :updateCurrentConfig
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateCurrentConfig(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT&V0\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT&V0\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getCurrentConfig");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}

/****************************************************************
*FUNCTION NAME:updateTaCapabilities
*FUNCTION     :updateTaCapabilities
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateTaCapabilities(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT+GCAP\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT+GCAP\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getTaCapabilities");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}








/****************************************************************
*FUNCTION NAME:updateIMEI
*FUNCTION     :updateIMEI
*INPUT        :addrs; 
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::updateIMEI(uint16_t addrs)
{	
	char      *strStart,*str1;
  strStart = (char*)&Sim80x.UsartRxBuffer[0];  
     
  str1 = strstr(strStart,"AT+CGSN\r\r\n");
  
  if(str1!=NULL)
    sscanf(str1,"AT+CGSN\r\r\n%s",Sim80x.IMEI);

  // update Flash IC here
		
  debugTerminal("Sim80x_getIMEI");
  
  memset(Sim80x.UsartRxBuffer,0,sizeof(Sim80x.UsartRxBuffer));
	
}

/****************************************************************
*FUNCTION NAME:GSM_IRQHandler
*FUNCTION     :GSM_IRQHandler
*INPUT        :
*OUTPUT       :
****************************************************************/
//#pragma vector = USART_VECTOR
GSM_IRQHANDLER {
  
  //Sim80x.UsartRxBuffer[index] = USART_ReceiveData(GSM_USART_CH);
  //index++;
  USART_ClearITPendingBit(GSM_USART_CH, USART_IT_RXNE);

  USART_ClearFlag(GSM_USART_CH, USART_FLAG_RXNE);
}

#endif

#endif
