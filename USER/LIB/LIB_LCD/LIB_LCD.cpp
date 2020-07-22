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

#ifdef LIB_LCD_H


#if (_USE_LCD == 1)

#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_LCD.h"
#include "LIB_LCD.h"

HAL_ERC2401602 lcd240160;

/****************************************************************/
/*
sbit	int0 = P3 ^ 2;
sbit key_add = P0 ^ 1;
sbit key_dec = P0 ^ 2;
bit log = 0; 
*/

uchar Contrast_level = 0x3a;

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :LCD register config //details refer datasheet of ST7586S//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::RegConfigSettings(void)
{

	lcd240160.write_com(ST7586S_SLEEP_OFF); // Sleep OFF
	delay_ms(1);
	lcd240160.write_com(ST7586S_DISP_OFF); // Display OFF
	delay_ms(1);

	lcd240160.write_com(ST7586S_VOP_SET); // Vop = B9h
	delay_ms(1);
	lcd240160.write_data(Contrast_level);
	delay_ms(1);

	lcd240160.write_data(ST7586S_SRESET);
	delay_ms(1);

	lcd240160.write_com(ST7586S_BIAS_SYSTEM_SET); // BIAS = 1/14
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);

	lcd240160.write_com(ST7586S_BOOSTLVL_SET); // Booster = x8
	delay_ms(1);
	lcd240160.write_data(0x07);
	delay_ms(1);

	lcd240160.write_com(ST7586S_ANALOG_EN); // Enable Analog Circuit
	delay_ms(1);
	lcd240160.write_data(0x1D);
	delay_ms(1);

	lcd240160.write_com(ST7586S_NLINE_INV_SET); // N-Line = 0
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);

	lcd240160.write_com(ST7586S_DISP_MODE_MONO); // Monochrome Mode
	delay_ms(1);

	lcd240160.write_com(ST7586S_DISP_DUTY_SET); // Duty Setting
	delay_ms(1);
	lcd240160.write_data(0x9F);
	delay_ms(1);

	lcd240160.write_com(ST7586S_DISPINV_OFF); // Display Inversion OFF
	delay_ms(1);

	lcd240160.write_com(ST7586S_FOSC_SET); // FOSC Set to No Divide
	lcd240160.write_data(0x00);

	lcd240160.write_com(ST7586S_TEMP_RANGE_SET); // Temperature Range Setting
	delay_ms(1);
	lcd240160.write_data(0x32); // TA -> 50 degC
	delay_ms(1);
	lcd240160.write_data(0x41); // TB -> 65 degC
	delay_ms(1);
	lcd240160.write_data(0x48); // TC -> 75 degC

	lcd240160.write_com(ST7586S_FRAME_MONO); // Frame Rate Setting
	delay_ms(1);
	lcd240160.write_data(0x08); // Frame Rate -> 77 Hz
	lcd240160.write_data(0x08); // Frame Rate -> 77 Hz
	lcd240160.write_data(0x08); // Frame Rate -> 77 Hz

	lcd240160.write_com(ST7586S_DDRAM_EN); // Enable DDRAM Interface
	delay_ms(1);
	lcd240160.write_data(0x02);
	delay_ms(1);

	lcd240160.write_com(ST7586S_DISP_CTRL); // Scan Direction Setting
	delay_ms(1);
	lcd240160.write_data(0xC0); //COM:C160--C1   SEG: SEG384-SEG1 Default : 0xC0
	delay_ms(1);

	lcd240160.write_com(ST7586S_COL_ADRS_SET); // Column Address Setting
	delay_ms(1);
	lcd240160.write_data(0x00); // SEG0 -> SEG384
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);
	lcd240160.write_data(0x7F);
	delay_ms(1);

	lcd240160.write_com(ST7586S_ROW_ADRS_SET); // Row Address Setting
	delay_ms(1);
	lcd240160.write_data(0x00); // COM0 -> COM160
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);
	lcd240160.write_data(0x00);
	delay_ms(1);
	lcd240160.write_data(0x9F);
	delay_ms(1);
}

/****************************************************************
*FUNCTION NAME:Reset
*FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::PoweronReset(void)
{
	GPIO_SetBits(LCD_GPIO_RST, LCD_PIN_RST);
	delay_ms(5);
	GPIO_ResetBits(LCD_GPIO_RST, LCD_PIN_RST);
	delay_ms(10);
	GPIO_SetBits(LCD_GPIO_RST, LCD_PIN_RST);
	delay_ms(200);
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :LCD initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::Init()
{
	lcd240160.SPIInit();
	lcd240160.Periph_Init();

	GPIO_SetBits(LCD_GPIO_NSS, LCD_PIN_NSS);
	delay_ms(50);

	PoweronReset();
	RegConfigSettings();

	lcd240160.write_com(ST7586S_DISP_ON); // Display ON
}

/****************************************************************
*FUNCTION NAME:display_black
*FUNCTION     :display a black screen
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::display_black() //all  display
{
	uint i, j;

	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < 160; i++)
	{
		for (j = 0; j < 120; j++) //1byte data or 2 dot  120*2=240dot
		{
			lcd240160.write_data(0xff);
		}
	}
}

/****************************************************************
*FUNCTION NAME:display_white
*FUNCTION     :display white screen
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::display_white() //clear display
{
	uint i, j;
	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < 160; i++)
	{
		for (j = 0; j < 120; j++) //1byte data or 2 dot  120*2=240dot
		{
			lcd240160.write_data(0x00);
		}
	}
}

/****************************************************************
*FUNCTION NAME:disppic
*FUNCTION     :disppic
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::disppic(unsigned char *pic)
{
	uint k = 0;
	uint i, j;
	uchar temp;
	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < 160; i++) // 240*160 B/W picture for example
	{
		for (j = 0; j < 30; j++) // 240 dot/ 8 bite=30 byte
		{
			temp = pic[k++];
			Data_processing(temp);
		}
	}
}
/****************************************************************
*FUNCTION NAME:dispchar
*FUNCTION     :dispchar
*INPUT        :none
*OUTPUT       :none
****************************************************************/

/****************************************************************
*FUNCTION NAME:text_dot
*FUNCTION     :
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::text_dot(uchar data1, uchar data2)
{
	uint i, j;
	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < 80; i++)
	{
		for (j = 0; j < 120; j++) //1byte data or 2 dot  120*2=240dot
		{
			lcd240160.write_data(data1);
		}
		for (j = 0; j < 120; j++)
		{
			lcd240160.write_data(data2);
		}
	}
}

/****************************************************************
*FUNCTION NAME:Data_processing
*FUNCTION     :Data_processing
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::Data_processing(uchar temp) // 4byte  data 8 dot  (B B X - A A X - X X X)
{
	unsigned char t1, t2;
	unsigned char d1, d2, d3, d4;

	t1 = (temp & 0x80) >> 3;
	t2 = (temp & 0x40) << 1;
	t1 = t1 | t1 >> 1;
	t2 = t2 | t2 >> 1;
	d1 = t1 | t2;

	t1 = (temp & 0x20) >> 1;
	t2 = (temp & 0x10) << 3;
	t1 = t1 | t1 >> 1;
	t2 = t2 | t2 >> 1;
	d2 = t1 | t2;

	t1 = (temp & 0x08) << 1;
	t2 = (temp & 0x04) << 5;
	t1 = t1 | t1 >> 1;
	t2 = t2 | t2 >> 1;
	d3 = t1 | t2;

	t1 = (temp & 0x02) << 3;
	t2 = (temp & 0x01) << 7;
	t1 = t1 | t1 >> 1;
	t2 = t2 | t2 >> 1;
	d4 = t1 | t2;

	lcd240160.write_data(d1);
	lcd240160.write_data(d2);
	lcd240160.write_data(d3);
	lcd240160.write_data(d4);
}

/****************************************************************
*FUNCTION NAME:display_Contrast_level
*FUNCTION     :display_Contrast_level
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::display_Contrast_level(uchar number)
{
	uchar number1, number2, number3;
	number1 = number / 100;
	number2 = number % 100 / 10;
	number3 = number % 100 % 10;

	write_com_number(0, 0, num, number1);

	write_com_number(4, 0, num, number2);

	write_com_number(8, 0, num, number3);
}

/****************************************************************
*FUNCTION NAME:display_address
*FUNCTION     :display_address
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::display_address(uchar x, uchar y)
{
	lcd240160.write_com(ST7586S_COL_ADRS_SET); // Column Address Setting
	lcd240160.write_data(0x00);				   // SEG0 -> SEG240
	lcd240160.write_data(x + 8);			   // SEG8*3=24
	lcd240160.write_data(0x00);
	lcd240160.write_data(0x7F);				   // SEG240/3=80  seg x(dont use)  seg n  seg n
	lcd240160.write_com(ST7586S_ROW_ADRS_SET); // Row Address Setting
	lcd240160.write_data(0x00);				   // COM0 -> COM160
	lcd240160.write_data(y);
	lcd240160.write_data(0x00);
	lcd240160.write_data(0x9F);
}
/****************************************************************
*FUNCTION NAME:set_address
*FUNCTION     :set_address
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::setAddress(uchar Xs, uchar Xe, uchar Ys, uchar Ye)
{
	// Setting DDRAM Address
	lcd240160.write_com(ST7586S_COL_ADRS_SET); // Column Address Setting
	lcd240160.write_data(0x00);				   // Xs Column Starting Address Xs15-Xs8
	lcd240160.write_data(Xs);				   // Xs Column Starting Address Xs7-Xs0
	lcd240160.write_data(0x00);				   // Xe Column Ending Address Xe15-Xe8
	lcd240160.write_data(Xe);				   // Xe Column Ending Address Xe7-Xe0

	lcd240160.write_com(ST7586S_ROW_ADRS_SET); // Row Address Setting
	lcd240160.write_data(0x00);				   // Ys Row Starting Address Ys15-Ys8
	lcd240160.write_data(Ys);				   // Ys Row Starting Address Ys7-Ys0
	lcd240160.write_data(0x00);				   // Ye Row Ending Address Ye15-Ye8
	lcd240160.write_data(Ye);				   // Ye Row Ending Address Ye7-Ye0
}

/****************************************************************
*FUNCTION NAME:setWindow
*FUNCTION     :setWindow
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::setWindow(uchar Xs, uchar Xe, uchar Ys, uchar Ye)
{
	// Setting DDRAM Address
	lcd240160.write_com(ST7586S_COL_ADRS_SET); // Column Address Setting
	lcd240160.write_data(0x00);				   // Xs Column Starting Address Xs15-Xs8
	lcd240160.write_data(Xs + 8);			   // Xs Column Starting Address Xs7-Xs0
	lcd240160.write_data(0x00);				   // Xe Column Ending Address Xe15-Xe8
	lcd240160.write_data(Xe + 8);			   // Xe Column Ending Address Xe7-Xe0

	lcd240160.write_com(ST7586S_ROW_ADRS_SET); // Row Address Setting
	lcd240160.write_data(0x00);				   // Ys Row Starting Address Ys15-Ys8
	lcd240160.write_data(Ys);				   // Ys Row Starting Address Ys7-Ys0
	lcd240160.write_data(0x00);				   // Ye Row Ending Address Ye15-Ye8
	lcd240160.write_data(Ye);				   // Ye Row Ending Address Ye7-Ye0
}

/****************************************************************
*FUNCTION NAME:setLine
*FUNCTION     :setLine
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::setLine(uchar Ys)
{
	// Setting DDRAM Address
	lcd240160.write_com(ST7586S_START_LINE); // Start Line
	lcd240160.write_data(Ys);				 // Xs Column Starting Address Xs7-Xs0
}

/****************************************************************
*FUNCTION NAME:drawLine_H
*FUNCTION     :drawLine
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::drawLine_H(uchar Xs, uchar Xe, uchar Ys, uchar Ye)
{
	uint8_t i, j;

	setAddress(Xs + 8, Xe + 8, Ys, Ye);
	//setLine(0);

	// Setting DDRAM Data
	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < (Ye - Ys); i++)
	{
		for (j = 0; j < (Xe - Xs); j++) //1byte data or 2 dot  120*2=240dot
		{
			lcd240160.write_data(0x18);
		}
	}
}
/****************************************************************
*FUNCTION NAME:drawLine_H
*FUNCTION     :drawLine
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::drawLine_V(uchar Xs, uchar Xe, uchar Ys, uchar Ye)
{
	uint8_t i;

	setAddress(Xs + 8, Xe + 7, Ys, Ye);

	// Setting DDRAM Data
	lcd240160.write_com(ST7586S_DISP_DATA_WRITE);
	for (i = 0; i < (Ye - Ys); i++)
	{
		lcd240160.write_data(0x18);
	}
}
/****************************************************************
*FUNCTION NAME:findCharacter
*FUNCTION     :findCharacter
*INPUT        :none
*OUTPUT       :none
****************************************************************/
tChar LCD34::findCharacter(uchar val, uint8_t &hpix, uint8_t &vpix)
{
	tChar img;
	const uint8_t *adrs;
	uint8_t len, i, size;

	len = EN.length;

	for (i = 0; i <= len; i++)
	{
		if (val == EN_array[i].code)
			break;
	}

	img.image = EN_array[i].image;
	adrs = img.image->name;
	hpix = img.image->Hpix;
	vpix = img.image->Vpix;
	size = img.image->Datablock;

	return img;
}

/****************************************************************
*FUNCTION NAME:setCharacter
*FUNCTION     :setCharacter
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::setCharacter(tChar val, uint8_t &hpix, uint8_t &vpix, uint8_t &xpos, uint8_t &ypos)
{
	uchar data[64];

	uchar temp;
	uint8_t i, j, k, size, xByte, hSpace;
	const uint8_t *adrs;

	adrs = val.image->name;
	hpix = val.image->Hpix;
	vpix = val.image->Vpix;
	size = val.image->Datablock;

	if (hpix > 8)
		size = (((hpix % 8) + 1) * vpix);
	else
		size = val.image->Vpix;

	for (i = 0; i <= size; i++)
	{
		data[i] = *adrs++;
	}

	if (hpix > 8)
		xByte = (hpix / 8) + 1;
	else
		xByte = 1;

	size = (xByte * vpix);

	for (j = 0; j < vpix; j++)
	{
		setAddress(xpos + 8, xpos + 8 + (xByte * 4), ypos + j, ypos + j + 1);

		// Setting DDRAM Data
		lcd240160.write_com(ST7586S_DISP_DATA_WRITE);

		for (i = 0; i < xByte; i++)
		{
			temp = data[k++];
			//temp = ~temp;
			Data_processing(temp);
		}
	}

	if (hpix < 8)
		hSpace = (hpix / 2) + (hpix % 2);
	else if (hpix > 8)
		hSpace = (hpix / 2) + 1;

	xpos = xpos + hSpace;
	//ypos = ypos + vpix;
}
/****************************************************************
*FUNCTION NAME:setIcon
*FUNCTION     :setIcon
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::setIcon(uchar *val, uint8_t hpix, uint8_t vpix, uint8_t &xpos, uint8_t &ypos)
{
	uchar temp;
	uint8_t i, j, k, size, xByte, hSpace;
	//const uint8_t *adrs;

	if ((hpix % 8) == 0)
		xByte = hpix / 8;
	else if (hpix < 8)
		xByte = 1;
	else if ((hpix > 8) & ((hpix % 8) != 0))
	{
		xByte = (hpix / 8) + 1;
	}

	size = (xByte * vpix);

	for (j = 0; j < vpix; j++)
	{
		setAddress(xpos + 8, xpos + 8 + (xByte * 4), ypos + j, ypos + j + 1);

		// Setting DDRAM Data
		lcd240160.write_com(ST7586S_DISP_DATA_WRITE);

		for (i = 0; i < xByte; i++)
		{
			temp = val[k++];
			//temp = ~temp;
			Data_processing(temp);
		}
	}
}
/****************************************************************
*FUNCTION NAME:write_com_number
*FUNCTION     :write_com_number
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void LCD34::write_com_number(uchar x, uchar y, uchar *n, uchar k)
{
	uchar i, j;

	lcd240160.write_com(0x2c);
	for (i = 0; i < 14; i++)
	{

		display_address(x, y);

		{
			j = (*(n + 14 * k + i));
			Data_processing(j);
		}
		lcd240160.write_data(0x00);
		y++;
	}
}

#endif

#endif