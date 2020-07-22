#include "stm32l1xx.h"  
#include "Settings.h"
#include "Shortcodes.h"

#ifndef HAL_IOEXP_H
#define HAL_IOEXP_H
  
#ifndef _USE_IOEXP
  #define _USE_IOEXP
#endif

#if(_USE_IOEXP==1)

#include "PCF8574_SHORTCODES.h"

#define PCF8574_OK          0x00
#define PCF8574_PIN_ERROR   0x81
#define PCF8574_I2C_ERROR   0x82

//*************************************** INSTRUCTION SET  **************************************************//
#define PCF8574_DEV_OPCODE					0x40
#define PCF8574_WRITE						0x00
#define PCF8574_READ						0x01


class HAL_PCF8574
{
public:
	//I2C - HAL
	void I2CInit(void);
	void I2CStart(void);
	void I2CStop(void);
	uint8_t I2CAddressdirection(uint8_t opcode, uint8_t direction);
	void I2CTransmit(uint8_t msg);
	uchar I2CReceive(void);
	uint8_t I2CReceiveACK(void);
	uint8_t I2CReceiveNACK(void);

	void resetDevice(void);
	void enableDevice(void);
	void Reset(void);

	void writeRegister(uint8_t address, uint8_t val);
	uchar readRegister(uint8_t address, uint8_t reg);
	uint8_t selectPin(uint8_t pin);
	void writePin(uint8_t address, uint8_t pin, uint8_t val);
	bool readPin(uint8_t address, uint8_t pin, uint8_t val);

	void releaseSDA(uint8_t address);

	//3rd party libraries
        /*
	explicit PCF8574(const uint8_t deviceAddress);
	void    begin(uint8_t val = 0xFF);

	uint8_t read8();
	uint8_t read(uint8_t pin);
	uint8_t value() const { return _dataIn; };

	void    write8(const uint8_t value);
	void    write(const uint8_t pin, const uint8_t value);
	uint8_t valueOut() const { return _dataOut; }

	//added 0.1.07/08 Septillion
	inline uint8_t readButton8()  { return PCF8574::readButton8(_buttonMask); }
	uint8_t        readButton8(const uint8_t mask = 0xFF);
	uint8_t        readButton(const uint8_t pin);
	inline void    setButtonMask(uint8_t mask) { _buttonMask = mask; };

	// rotate, shift, toggle, reverse expect all lines are output
	void    toggle(const uint8_t pin);
	void    toggleMask(const uint8_t mask = 0xFF);    // default 0xFF ==> invertAll()
	void    shiftRight(const uint8_t n = 1);
	void    shiftLeft(const uint8_t n = 1);
	void    rotateRight(const uint8_t n = 1);
	void    rotateLeft(const uint8_t n = 1);
	void    reverse();

	int lastError();

private:
    uint8_t _address;
    uint8_t _dataIn;
    uint8_t _dataOut;
    uint8_t _buttonMask;
    int     _error;
*/
};

extern HAL_PCF8574 ioexp8;

// -- END OF FILE --

#endif

#endif