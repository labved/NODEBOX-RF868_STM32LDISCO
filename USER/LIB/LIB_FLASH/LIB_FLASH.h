#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_FLASH
#define _USE_FLASH
#endif

#if (_USE_FLASH == 1)

//*************************************** DEFINE **************************************************//
#define uchar unsigned char

//*************************************** DEFINE PARAMETERS**************************************************//
#define SFLASH_ADDRESS_SIZE 3
#define SFLASH_PAGE_SIZE 256
#define SFLASH_SECTOR_SIZE 4096
#define SFLASH_BLOCK_SIZE 65536

//*************************************** INSTRUCTION SET  **************************************************//

#define SFLASH_WRITE_EN 0X06  // Write enable
#define SFLASH_WRITE_DIS 0X04 // Write disable
#define SFLASH_READ_SR1 0x05  // Read Status Register 1
#define SFLASH_READ_SR2 0x35  // Read Status Register 2
#define SFLASH_READ_SR3 0x15  // Read Status Register 2

#define SFLASH_WRITE_SR1 0x01		  // Write Status Register 1
#define SFLASH_WRITE_SR2 0x31		  // Write Status Register 2
#define SFLASH_WRITE_SR3 0x11		  // Write Status Register 3
#define SFLASH_WRITE_SR_VOLATILE 0x50 // Write Volatile Status Register

// WRITE
#define SFLASH_PROG_PAGE 0X02	   // Program Page
#define SFLASH_PROG_PAGE_QUAD 0x32 // Program Quad Page

// ERASE
#define SFLASH_ERASE_BLOCK64 0xD8 // Erase Block 64 KB
#define SFLASH_ERASE_BLOCK32 0x52 // Erase Block 32 KB
#define SFLASH_ERASE_SECTOR 0x20  // Erase Sector 4KB
#define SFLASH_ERASE_CHIP 0x60	  // Erase entire Chip
#define SFLASH_ERASE_SUSPEND 0x75 // Erase suspend
#define SFLASH_ERASE_RESUME 0x7A  // Erase resume
#define SFLASH_PWR_DOWN 0xB9	  // Power down

// MODE
#define SFLASH_MODE_HIGH_PERF	0xA3 // High performance mode
#define SFLASH_MODE_CONT_READ 	0xFF // Continuous Read Mode
#define SFLASH_DEV_ID 0xAB		   // Read Device ID
#define SFLASH_MAN_ID 0x90		   // Read Manufacturere ID
#define SFLASH_UNIQUE_ID 0x4B	   // Read unique ID
#define SFLASH_JEDEC_ID 0x9F	   // Read JEDEC ID

// READ
#define SFLASH_READ_DATA 0x03			  // Read data
#define SFLASH_FAST_READ_DATA 0x0B		  // Fast Read data
#define SFLASH_FAST_READ_DUAL_OUTPUT 0x3B // Fast Read dual output
#define SFLASH_FAST_READ_DUAL_IO 0xBB	  // Fast Read dual IO
#define SFLASH_FAST_READ_QUAD_OUTPUT 0x6B // Fast Read quad output
#define SFLASH_FAST_READ_QUAD_IO 0xEB	  // Fast Read quad IO
#define SFLASH_OCT_READ_QUAD_IO 0xE3	  // Octal word Read quad IO

// MASK
#define SFLASH_ADDRS_MASK_A0 0x00FF0000 // MSB
#define SFLASH_ADDRS_MASK_A1 0x0000FF00
#define SFLASH_ADDRS_MASK_A2 0x000000FF // LSB

#define SFLASH_SR1_MASK_BUSY 0x01

//*************************************** FUNCTIONAL PROTOTYPES **************************************************//

class LIB_QFLASH
{
public:
	void PoweronReset(void);
	void RegConfigSettings(void);
	void Init(void);

	uchar readStatusRegister(uchar reg);
	void writeStatusRegister(uchar reg, uchar val);
	void readData(uchar *val, uchar *addrs, uint32_t bytes);
	void fastReadData(uchar *val, uchar *addrs, uint32_t bytes);
	void pageProgram(uchar *val, uchar *addrs, uint32_t bytes);
	void eraseSector(uchar *addrs);
	void eraseBlock32K(uchar *addrs);
	void eraseBlock64K(uchar *addrs);
	void eraseChip(void);
	void powerDown(void);
	void wakeUp(void);

	void getDeviceID(uchar &val);
	void getManufacturerID(uchar *val);
	void getUniqueID(uchar *val);
	void getJedecID(uchar *val);

	void addressto3BYTES(uint32_t addrs, uchar *val);
};

extern LIB_QFLASH qFlash;

#endif