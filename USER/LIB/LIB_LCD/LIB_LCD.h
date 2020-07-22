/*
	ELECHOUSE_CC1101.cpp - CC1101 module library
	Copyright (c) 2010 Michael.
  	Author: Michael, <www.elechouse.com>
  	Version: November 12, 2010

	This library is designed to use CC1101/CC1100 module on Arduino platform.
	CC1101/CC1100 module is an useful wireless module.Using the functions of the 
	library, you can easily send and receive data by the CC1101/CC1100 module. 
	Just have fun!
	For the details, please refer to the datasheet of CC1100/CC1101.
*/

#include "stm32l1xx.h"
#include "Settings.h"

#ifndef LIB_LCD_H
  #define LIB_LCD_H

#if (_USE_LCD == 1)

#include "EN.c"

//***************************************LCD define**************************************************//
#define byte char
#define uint unsigned int
#define uchar unsigned char

extern uchar num[];
extern uchar logo[];
extern uchar logo1[];
extern uchar home_screen[];
extern uchar NORM_8[];
extern uchar BOLD_8[];

//ST7586S Strobe commands
#define ST7586S_NOP 0x00		 // No Operation.
#define ST7586S_SRESET 0x01		 // Software Reset.
#define ST7586S_SLEEP_ON 0x10	 // Power Save ON.
#define ST7586S_SLEEP_OFF 0x11	 // Power Save OFF.
#define ST7586S_PARMODE_ON 0x12	 // Partial Mode ON
#define ST7586S_PARMODE_OFF 0x13 // Partial Mode OFF

#define ST7586S_PIX_ON 0X22	 // All Pixel OFF
#define ST7586S_PIX_OFF 0X23 // All Pixel ON

#define ST7586S_PART_DISP_AREA 0X30 // Set Partial Display Area.
#define ST7586S_SCROLL_AREA 0x33	// Set Scroll Area.

#define ST7586S_COL_ADRS_SET 0x2A // Set Column Address.
#define ST7586S_ROW_ADRS_SET 0x2B // Set Row Address.

#define ST7586S_DISP_OFF 0X28		 // Display OFF
#define ST7586S_DISP_ON 0X29		 // Display ON
#define ST7586S_DISPINV_OFF 0x20	 // Display Inverse OFF
#define ST7586S_DISPINV_ON 0x21		 // Display Inverse ON
#define ST7586S_DISP_DATA_WRITE 0x2C // Write Display Data
#define ST7586S_DISP_DATA_READ 0x2E	 // Read Display Data
#define ST7586S_DISP_CTRL 0x36		 //	Display Control.
#define ST7586S_START_LINE 0x37		 // Start Line.
#define ST7586S_DISP_MODE_GRAY 0x38	 //	Display Mode : Gray Mode
#define ST7586S_DISP_MODE_MONO 0x39	 //	Display Mode : Monochrome Mode
#define ST7586S_DDRAM_EN 0x3A		 //  DDRAM ENABLE

#define ST7586S_DISP_DUTY_SET 0xB0	  // Display Duty SET.
#define ST7586S_COM_FIRST_SET 0xB1	  // COM First Output SET.
#define ST7586S_FOSC_SET 0xB3		  // FOSC Divider Ratio SET.
#define ST7586S_DISP_PARTIAL_SET 0xB4 // Partial Display SET.
#define ST7586S_NLINE_INV_SET 0xB5	  // New Line Inversion SET.
#define ST7586S_RMW_EN 0xB8			  // Read Modify Write ENABLE.
#define ST7586S_RMW_DIS 0xB9		  // Read Modify Write DISABLE.

#define ST7586S_VOP_SET 0xC0		 // VOP SET.
#define ST7586S_VOP_STEP_INC 0xC1	 // VOP Step INCREASE.
#define ST7586S_VOP_STEP_DEC 0xC2	 // VOP Step DECREASE.
#define ST7586S_BIAS_SYSTEM_SET 0xC3 // Bias System SET.
#define ST7586S_BOOSTLVL_SET 0xC4	 // Set Boost Level.
#define ST7586S_VOP_OFFSET_SET 0xC7	 // Set VOP Offset.
#define ST7586S_ANALOG_EN 0xD0		 // Enable Analog Control.
#define ST7586S_AUTO_RCTRL_EN 0x8F	 // Enable Auto Read.
#define ST7586S_AUTO_RCTRL_DIS 0x9F	 // Disable Auto Read.

#define ST7586S_OTP_WRRD_CTRL_READ 0x00	 // OTP WR/RD Control - Read.
#define ST7586S_OTP_WRRD_CTRL_WRITE 0x20 // OTP WR/RD Control - Write.
#define ST7586S_OTP_CTRL_OUT 0xE1		 // OTP Control Out.
#define ST7586S_OTP_WRITE 0xE2			 // OTP Write.
#define ST7586S_OTP_READ 0xE3			 // OTP Read.
#define ST7586S_OTP_DIS 0x19			 // OTP Disable.
#define ST7586S_OTP_EN 0x59				 // OTP Enable.

#define ST7586S_FRAME_GRAY 0xF0 // Frame Rate Gray Command.
#define ST7586S_FRAME_MONO 0xF1 // Frame Rate Monochrome Command.

#define ST7586S_TEMP_RANGE_SET 0XF2		// Set Temperature Range Setting.
#define ST7586S_TEMP_GRAD_COMP_SET 0xF4 //	Set Temperature Gradient Setting.

//************************************* class **************************************************//
class LCD34
{
public:
	void PoweronReset(void);
	void RegConfigSettings(void);
	void Init();
	void display_black();
	void display_white();
	void disppic(uchar *pic);

	void text_dot(uchar, uchar);

public:
	void write_com_number(uchar x, uchar y, uchar *n, uchar k);

	void Data_processing(uchar temp);
	void display_Contrast_level(uchar number);
	void display_address(uchar x, uchar y);
	void setAddress(uchar Xs, uchar Xe, uchar Ys, uchar Ye);
	void setWindow(uchar Xs, uchar Xe, uchar Ys, uchar Ye);

	void setLine(uchar Ys);
	tChar findCharacter(uchar val, uint8_t &hpix, uint8_t &vpix);
	void setCharacter(tChar val, uint8_t &hpix, uint8_t &vpix, uint8_t &xpos, uint8_t &ypos);
	void setIcon(uchar *val, uint8_t hpix, uint8_t vpix, uint8_t &xpos, uint8_t &ypos);

	// Shapes
	void drawLine_H(uchar Xs, uchar Xe, uchar Ys, uchar Ye);
	void drawLine_V(uchar Xs, uchar Xe, uchar Ys, uchar Ye);
};

extern LCD34 lcd;
#endif

#endif