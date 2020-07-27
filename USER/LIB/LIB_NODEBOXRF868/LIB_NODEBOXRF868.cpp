#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "stdint.h"
#include "string.h"
#include "stdlib.h"

#include "LIB_NODEBOXRF868.h"
#include "LIB_SDMMC.h"
#include "LIB_FLASH.h"
#include "LIB_LCD.h"
#include "LIB_HC05.h"
#include "LIB_SIM800C.h"
#include "HAL_PCF8574.h"
#include "LIB_E70-868T30S.h"

#include "PCF8574_SHORTCODES.h"
#include "HC05_TYPEDEF.h"

SD_MMC sdCard;
LIB_QFLASH w25qFlash;
LCD34 lcd1;
LIB_HC05 bluetooth;
LIB_SIM800C gsm;
HAL_PCF8574 ioexp8;

// HC05 for testing
BluetoothAddress adrs;
HC05_Role role;
uint32_t clss,iac,speed;
HC05_InquiryMode Inquiry;
HC05_Parity Parity;
HC05_Connection connection_mode;
HC05_Security security;
HC05_Encryption encryption;
HC05_State state;
InquiryCallback callback;
int16_t maxDevice;
uint16_t inquiry_interval,  inquiry_duration, paging_interval, paging_duration;
uint16_t max_time, min_time, retry_interval, sniff_timeout;
uint8_t maxDuration, stop_bits, port_num, port_state;
uint8_t device_count;
uint16_t port_states;

bool led_sts,led_connection;

// used for testing W25Q
unsigned char manfID, devID, uniqueId, jedecID; 
char buf[30];

// PCF8574T Testing
char reg;


// for testing LCD
tChar test1;            
uchar Hpix, Vpix, Xpos, Ypos, i, len;
unsigned char gImage_bat[24] = { 
	0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XE0,0X80,0X20,0X80,0X30,0X80,0X30,0X80,0X20,
	0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00, };
unsigned char gImage_signal[24] = { 
	0X00,0X00,0X00,0X60,0X00,0X60,0X03,0X60,0X03,0X60,0X1B,0X60,0X1B,0X60,0X1B,0X60,
	0XDB,0X60,0XDB,0X60,0XDB,0X60,0X00,0X00, };
unsigned char gImage_bluetooth[24] = { 
	0X00,0X00,0X1F,0X80,0X3F,0X80,0X3D,0XC0,0X3F,0XC0,0X39,0XC0,0X39,0XC0,0X3F,0XC0,
	0X3D,0XC0,0X1F,0X80,0X1F,0X80,0X00,0X00, };

const char alpha[256] = { "Welcome to EngineerIdea... Translating ideas to realities" };


/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::Init(void)
{

 // uint8_t clk_src;

  // clock setting for system clock
  RCC_DeInit();
  RCC_HSICmd(ENABLE);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  //clk_src = RCC_GetSYSCLKSource();

  RCC_HCLKConfig(RCC_SYSCLK_Div1); // AHB to run at 16MHZ
  RCC_PCLK1Config(RCC_HCLK_Div2);  // APB1  to run at 8MHZ
  RCC_PCLK2Config(RCC_HCLK_Div2);  // APB2  to run at 8MHZ

  // clock setting for GPIO PORT
  RCC_AHBPeriphClockCmd(LED_AHB, ENABLE);
  RCC_APB2PeriphClockCmd(LED_APB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = LEDON_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_400KHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(LED_GPIO, &GPIO_InitStruct);

  
}

/****************************************************************
*FUNCTION NAME:testSDMMC
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testSDMMC(char *buffer)                           
{
    errCode = FR_OK;
    errCode = sdCard.makeVol();
    errCode = FR_OK;
    errCode = sdCard.makeDir(DIR_LOG);
    
    errCode = FR_OK;
    errCode = sdCard.filburstWrite(FILE_OBJ_LOG, FILE_PATH_LOG, buffer);

    errCode = FR_OK;
    errCode = sdCard.filburstWrite(FILE_OBJ_LOG, FILE_PATH_LOG, "\r\n");
    
    errCode = FR_OK;
    sdCard.filClose(FILE_OBJ_LOG);
    errCode = FR_OK;

}

/****************************************************************
*FUNCTION NAME:testW25Q
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testW25Q(void)                           
{

  w25qFlash.Init();
  delay_ms(100);
  w25qFlash.getManufacturerID(&manfID);
  w25qFlash.getDeviceID(devID);
  w25qFlash.getUniqueID(&uniqueId);
  w25qFlash.getJedecID(&jedecID);
}

/****************************************************************
*FUNCTION NAME:testLCD
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testLCD(void)                           
{

	lcd1.Init();
	lcd1.display_address(0, 0);
	lcd1.display_black();
	lcd1.display_address(0, 0);
	lcd1.display_white();
        
	Xpos = 1;
	Ypos = 1;
        
        len = strlen(alpha);
        
	for (i = 0; i < len; i++)
	{
		test1 = lcd1.findCharacter(alpha[i], Hpix, Vpix);
		lcd1.setCharacter(test1, Hpix, Vpix, Xpos, Ypos);
	}

	//lcd1.write_com(ST7586S_DISPINV_ON);
	//lcd1.write_com(ST7586S_DISPINV_OFF);   

	Xpos = 90;
	Ypos = 2;
	lcd1.setIcon(gImage_bluetooth, 12, 12, Xpos, Ypos);
        
	Xpos = 100;
	Ypos = 2;
	lcd1.setIcon(gImage_signal, 12, 12, Xpos, Ypos); 
	Xpos = 110;
	Ypos = 2;
	lcd1.setIcon(gImage_bat, 12, 12, Xpos, Ypos);        
        
	lcd1.display_address(0, 0);
	lcd1.display_black();
	lcd1.display_address(0, 0);
	lcd1.display_white();

                
	//   SHAPES
	lcd1.drawLine_H(0, 120, 0, 1);             // Horizontal Line 1 Row 240px
	lcd1.display_address(0, 0);
	lcd1.display_white();
	lcd1.drawLine_V(0, 1, 0, 160);             // Vertical Line 1 Column Row 160px  delay_ms(100);
  
}


/****************************************************************
*FUNCTION NAME:testHC05
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testHC05(void)                           
{
    bluetooth.Init();
    
    bluetooth.test();
    
    // Testing Data Mode
    //bluetooth.setDataMode();
    //bluetooth.getData(buf, sizeof(buf));
    //bluetooth.sendData("Hello World\r\n");

    //GPIO_SetBits(HC05_GPIO_EN, HC05_PIN_EN);    // data mode
    //GPIO_ResetBits(HC05_GPIO_EN, HC05_PIN_EN);        // command mode

    // Settings Commands
    // bluetooth.testCommand(); //checked

    // bluetooth.getVersion(buf, 30, 200); //checked
    // clearbuffer(buf, sizeof(buf));

    // bluetooth.getName(buf, 200); //checked
    // clearbuffer(buf, sizeof(buf));

    // bluetooth.setName("Nodebox", 200);//checked

    // bluetooth.getName(buf, 200); //checked
    // clearbuffer(buf, sizeof(buf));

    // UART SETTINGS - BAUDRATE
    // bluetooth.getSerialMode(speed, stop_bits, Parity,200);//checked

    // bluetooth.setSerialMode(speed, stop_bits, Parity,200);//checked
    // bluetooth.getSerialMode(speed, stop_bits, Parity,200);//checked

    // Roles
    // bluetooth.getRole(role, 200);  //checked

    // bluetooth.setRole(role, 200); //checked
    // bluetooth.getRole(role, 200); //checked

    // Device Class
    // bluetooth.getDeviceClass(clss, 200);  //checked

    // bluetooth.setDeviceClass(clss, 200); //checked
    // bluetooth.getDeviceClass(clss, 200); //checked

    // Inquiry Access Code
    // bluetooth.getInquiryAccessCode( iac, 200); //checked
    // bluetooth.setInquiryAccessCode( iac, 200);//checked
    // bluetooth.getInquiryAccessCode( iac, 200);//checked

    // Inquiry Mode
    // bluetooth.getInquiryMode(Inquiry, maxDevice, maxDuration, 200);//checked
    // bluetooth.setInquiryMode(Inquiry , maxDevice, maxDuration, 200);//checked
    // bluetooth.getInquiryMode(Inquiry , maxDevice, maxDuration, 200);//checked

    // Password
    // bluetooth.getPassword(buf, 200);//checked
    // clearbuffer(buf, sizeof(buf));
    // bluetooth.setPassword("password", 200);//checked
    // bluetooth.getPassword(buf, 200);//checked
    // clearbuffer(buf, sizeof(buf));
    
    // CONNECTION MODE
    // bluetooth.getConnectionMode(connection_mode,  200);//checked
    // bluetooth.setConnectionMode(connection_mode,  200);//checked
    // bluetooth.getConnectionMode(connection_mode,  200);//checked

    // LED
    // bluetooth.getLeds(led_sts,led_connection, 200);//checked
    // bluetooth.setLeds(led_sts,led_connection, 200);//checked
    // bluetooth.setLeds(led_sts,led_connection, 200);//checked

    // PORT
    // bluetooth.setPortState(port_num, port_state, 200);
    // bluetooth.getMultiplePorts(port_states, 200);
    // bluetooth.setMultiplePorts(port_states, 200);

    // INQUIRY PAGING PARAMS
    // bluetooth.getInquiryAndPagingParams(inquiry_interval, inquiry_duration, paging_interval, paging_duration, 200);//checked
    // bluetooth.setInquiryAndPagingParams(inquiry_interval, inquiry_duration, paging_interval, paging_duration, 200);//checked
    // bluetooth.getInquiryAndPagingParams(inquiry_interval, inquiry_duration, paging_interval, paging_duration, 200);//checked

    // SCAN PARAMETERS
    // bluetooth.getSniffParams(max_time, min_time, retry_interval, sniff_timeout, 200);//checked
    // bluetooth.setSniffParams(max_time, min_time, retry_interval, sniff_timeout, 200);//checked
    // bluetooth.getSniffParams(max_time, min_time, retry_interval, sniff_timeout, 200);//checked

    // SECURITY & ENCRYPTION
    // bluetooth.getSecurityAndEncryption(security, encryption, 200);//checked
    // bluetooth.setSecurityAndEncryption(security, encryption, 200);//checked
    // bluetooth.getSecurityAndEncryption(security, encryption, 200);//checked



    // STATES
    // bluetooth.getState(state, 200); //checked
    // bluetooth.initSerialPortProfile(200);//checked
    // bluetooth.inquire(callback, 200);
    // bluetooth.cancelInquiry(200);//checked
    // bluetooth.pair(adrs, 200);  
    // bluetooth.connect(adrs, 200);//checked
    // bluetooth.disconnect(200);//checked

    
    // To TEST
    // bluetooth.getAddress(adrs, 200);     //checked
    // bluetooth.getRemoteDeviceName(adrs, buf, sizeof(buf), 200);        //added size of as strlen 
    // // bluetooth.getLastAuthenticatedDevice(adrs, 200);//checked
    // bluetooth.bind(adrs, 200);
    // bluetooth.getAddressBound(adrs, 200);
    // bluetooth.enterSniffMode(200);
    // bluetooth.exitSniffMode(200);   
    // bluetooth.findDeviceInList(adrs, 200);
    // bluetooth.countDevicesInList(device_count, 200);//checked

    // bluetooth.deleteDeviceFromList(adrs, 200);
    // bluetooth.findDeviceInList(adrs, 200);
    // bluetooth.countDevicesInList(device_count, 200);  //checked

    // bluetooth.deleteAllDevicesFromList(200); //checked
    // bluetooth.findDeviceInList(adrs, 200);
    // bluetooth.countDevicesInList(device_count, 200);  //checked
  
}

/****************************************************************
*FUNCTION NAME:testIOEXP8
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testIOEXP8(void)                           
{
    bool test=0;
    uint8_t val=0;
    
    ioexp8.I2CInit();
    
    
   ioexp8.writeRegister(PCF8574_DEV7_ADDR, 0);  
   reg = ioexp8.readRegister(PCF8574_DEV7_ADDR, reg);
   
   ioexp8.writeRegister(PCF8574_DEV7_ADDR, 1);  
   reg = ioexp8.readRegister(PCF8574_DEV7_ADDR, reg);
   test = ioexp8.readPin(PCF8574_DEV7_ADDR,0x00, val);
   
   ioexp8.writePin(PCF8574_DEV7_ADDR,0x00, 0);
   reg = ioexp8.readRegister(PCF8574_DEV7_ADDR, reg);
   test = ioexp8.readPin(PCF8574_DEV7_ADDR,0x00, val);

    
}

/****************************************************************
*FUNCTION NAME:testGSM
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testGSM(void)                           
{     
  gsm.Init();
  
  //gsm.test();
  gsm.setFactoryDefault();
  gsm.setBaudRate(9600);
  gsm.setAtEcho(1);
  gsm.getIMEI(Sim80x.IMEI);

}

/****************************************************************
*FUNCTION NAME:testLORA
*FUNCTION     :ST7586S Strobe Command
*INPUT        :strobe: command; //refer defines in header file
*OUTPUT       :none
****************************************************************/
void NODEBOX_RF868::testLORA(void)                           
{

  
}