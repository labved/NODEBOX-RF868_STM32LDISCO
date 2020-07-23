#include "stm32l1xx.h"  
#include "Settings.h"

#ifdef HAL_IOEXP_H
  
#ifndef _USE_IOEXP
  #define _USE_IOEXP
#endif

#if(_USE_IOEXP==1)


#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_PCF8574.h"
#include "PCF8574_SHORTCODES.h"

/****************************************************************
*FUNCTION NAME:I2CInit
*FUNCTION     :I2C communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::I2CInit(void)
{
      // Setting GPIO for SCL
	RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);

	GPIO_InitTypeDef I2C_PCF8574_GPIO_SCL_InitStruct;

	I2C_PCF8574_GPIO_SCL_InitStruct.GPIO_Pin = PCF8574_PIN_SCL;
	I2C_PCF8574_GPIO_SCL_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	I2C_PCF8574_GPIO_SCL_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	I2C_PCF8574_GPIO_SCL_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	I2C_PCF8574_GPIO_SCL_InitStruct.GPIO_OType = GPIO_OType_OD;

	GPIO_Init(PCF8574_GPIO_I2C, &I2C_PCF8574_GPIO_SCL_InitStruct);


	// Setting GPIO for SDA
	RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);

	GPIO_InitTypeDef I2C_PCF8574_GPIO_SDA_InitStruct;

	I2C_PCF8574_GPIO_SDA_InitStruct.GPIO_Pin = PCF8574_PIN_SDA;
	I2C_PCF8574_GPIO_SDA_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	I2C_PCF8574_GPIO_SDA_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	I2C_PCF8574_GPIO_SDA_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	I2C_PCF8574_GPIO_SDA_InitStruct.GPIO_OType = GPIO_OType_OD;

	GPIO_Init(PCF8574_GPIO_I2C, &I2C_PCF8574_GPIO_SDA_InitStruct);

	//Setting AF for SCL,SDA
	GPIO_PinAFConfig(PCF8574_GPIO_I2C, PCF8574_AF_PIN_SCL, I2C_PCF8574_GPIO_AF);					//SCL
	GPIO_PinAFConfig(PCF8574_GPIO_I2C, PCF8574_AF_PIN_SDA, I2C_PCF8574_GPIO_AF);					//SDA



	// Configuring SPI Settings
	I2C_Cmd(PCF8574_I2C_CH, DISABLE);

	I2C_InitTypeDef  I2C_PCF8574_InitStruct;

	I2C_DeInit(PCF8574_I2C_CH);														// DeInitialize I2C Module

	I2C_StructInit(&I2C_PCF8574_InitStruct);										// Setting I2C with default values

	I2C_PCF8574_InitStruct.I2C_ClockSpeed = I2C_PCF8574_CLK_SPEED;
	I2C_PCF8574_InitStruct.I2C_Mode = I2C_PCF8574_MODE;
	I2C_PCF8574_InitStruct.I2C_DutyCycle = I2C_PCF8574_DUTY_CYCLE;
	I2C_PCF8574_InitStruct.I2C_OwnAddress1 = I2C_PCF8574_OWN_ADDR;
	I2C_PCF8574_InitStruct.I2C_Ack = I2C_PCF8574_ACK;
	I2C_PCF8574_InitStruct.I2C_AcknowledgedAddress = I2C_PCF8574_ACK_ADDR;

	I2C_Init(PCF8574_I2C_CH, &I2C_PCF8574_InitStruct);
        //I2C_StretchClockCmd(PCF8574_I2C_CH, DISABLE);

	//void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);
	//void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address);
	//void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
	//I2C_NACKPositionConfig(PCF8574_I2C_CH, 8);

	I2C_Cmd(PCF8574_I2C_CH, ENABLE);
        

        
}

/****************************************************************
*FUNCTION NAME:I2CStart
*FUNCTION     :I2CStart
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::I2CStart(void)
{
	// Wait until I2Cx is not busy anymore
	while (I2C_GetFlagStatus(PCF8574_I2C_CH, I2C_FLAG_BUSY));

	// Generate start condition
	I2C_GenerateSTART(PCF8574_I2C_CH, ENABLE);

	// Wait for I2C EV5. 
	// It means that the start condition has been correctly released 
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_MODE_SELECT));
}
/****************************************************************
*FUNCTION NAME:I2CStop
*FUNCTION     :I2CStop
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::I2CStop(void)
{
	// Generate I2C stop condition
	I2C_GenerateSTOP(PCF8574_I2C_CH, ENABLE);
	// Wait until I2C stop condition is finished
	while (I2C_GetFlagStatus(PCF8574_I2C_CH, I2C_FLAG_STOPF));
}

/****************************************************************
*FUNCTION NAME:I2CAddressdirection
*FUNCTION     :I2CAddressdirection 
*INPUT        :Device Opcode, direction
*OUTPUT       :flag
****************************************************************/
uint8_t HAL_PCF8574::I2CAddressdirection(uint8_t opcode, uint8_t direction)
{
	uint8_t error;
	// Send slave address
	I2C_Send7bitAddress(PCF8574_I2C_CH, opcode, direction);

	error = I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_SLAVE_ACK_FAILURE);		// Wait for I2C EV6
		// It means that a slave acknowledges his address

	if (!error)
	{

		if (direction == I2C_Direction_Transmitter)
		{
			while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		}
		else if (direction == I2C_Direction_Receiver)
		{
			while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		}
		return 0;
	}

	else
		return 1;
}

/****************************************************************
*FUNCTION NAME:I2CTransmit
*FUNCTION     :I2CTransmit
*INPUT        :byte
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::I2CTransmit(uint8_t msg)
{
	// Send data byte
	I2C_SendData(PCF8574_I2C_CH, msg);
	// Wait for I2C EV8_2.
	// It means that the data has been physically shifted out and 
	// output on the bus)
	while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/****************************************************************
*FUNCTION NAME:I2CReceive
*FUNCTION     :I2CReceive
*INPUT        :none
*OUTPUT       :value
****************************************************************/
uchar HAL_PCF8574::I2CReceive(void)
{
	uchar val;

	// Wait for I2C EV7.
	// It means that the data has been physically shifted out and 
	// output on the bus)
	while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Send data byte
	val = I2C_ReceiveData(PCF8574_I2C_CH);

	return val;
}

/****************************************************************
*FUNCTION NAME:I2CReceiveACK
*FUNCTION     :I2CReceiveACK
*INPUT        :none
*OUTPUT       :data
****************************************************************/
uint8_t HAL_PCF8574::I2CReceiveACK(void)
{
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Enable ACK of received data
	I2C_AcknowledgeConfig(PCF8574_I2C_CH, ENABLE);

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(PCF8574_I2C_CH);
}

/****************************************************************
*FUNCTION NAME:I2CReceiveNACK
*FUNCTION     :I2CReceiveNACK
*INPUT        :none
*OUTPUT       :data
****************************************************************/
uint8_t HAL_PCF8574::I2CReceiveNACK(void)
{
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(PCF8574_I2C_CH, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Disable ACK of received data
	I2C_AcknowledgeConfig(PCF8574_I2C_CH, DISABLE);

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(PCF8574_I2C_CH);
}

/****************************************************************
*FUNCTION NAME:resetDevice
*FUNCTION     :resetDevice
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::resetDevice(void)
{
	//GPIO_WriteBit(PCF8574_GPIO_RST, PCF8574_PIN_RST, Bit_SET);
	delay_ms(1);
}

/****************************************************************
*FUNCTION NAME:enableDevice
*FUNCTION     :enableDevice
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::enableDevice(void)
{
	//GPIO_WriteBit(PCF8574_GPIO_RST, PCF8574_PIN_RST, Bit_RESET);
	delay_ms(1);
}

/****************************************************************
*FUNCTION NAME:Reset
*FUNCTION     :Reset
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::Reset(void)
{
	resetDevice();
	delay_ms(1);
	enableDevice();
	delay_ms(1);
}

/****************************************************************
*FUNCTION NAME:writeRegister
*FUNCTION     :writeRegister
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::writeRegister(uint8_t address, uint8_t val)
{
	uint8_t flag;

	I2CStart();
        
        uint8_t param = (PCF8574_DEV_OPCODE | PCF8574_WRITE) | (address << 1);
	flag = I2CAddressdirection( param, I2C_Direction_Transmitter);
	
	if(flag == 0)
	{
		//I2CTransmit(reg);
		I2CTransmit(val);
		I2CStop();
	}
	else
	{
		I2CStop();
	}
}

/****************************************************************
*FUNCTION NAME:readRegister
*FUNCTION     :readRegister
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
uchar HAL_PCF8574::readRegister(uint8_t address, uint8_t reg)
{
	uchar val[1];

	I2CStart();
	I2CAddressdirection((PCF8574_DEV_OPCODE | PCF8574_READ) | (address << 1) , I2C_Direction_Transmitter);
	//I2CTransmit(reg);
	I2CStop();

	I2CStart();
	I2CAddressdirection((PCF8574_DEV_OPCODE | PCF8574_READ) | (address << 1), I2C_Direction_Receiver);
	val[0] = I2CReceiveNACK();
	I2CStop();

	return val[0];
}

/****************************************************************
*FUNCTION NAME:readRegister
*FUNCTION     :readRegister
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::releaseSDA(uint8_t address)
{
	if (!GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA))
	{
		I2CAddressdirection((PCF8574_DEV_OPCODE | address) << 1, I2C_Direction_Transmitter);
		I2CStop();
	}


	RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);

  	RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);



	/*Workaround p26 errata STM32F103*/
	I2C_Cmd(PCF8574_I2C_CH, DISABLE);

	GPIO_InitTypeDef GPIO_PCF8574_I2C_InitStruct;

	//OD HIGHG
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SCL | PCF8574_PIN_SDA;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	//GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
        GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);

	GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL, Bit_SET);
	GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA, Bit_SET);

	while (!((GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL) == Bit_SET)
		&& (GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA) == Bit_SET)));

	//OD SDA LOW
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SDA;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	//	GPIO_PCF8574_I2C_InitStruct.Pull = GPIO_PULLDOWN;
	
        GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);
	
        GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA, Bit_RESET);
	
        while (GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA) != Bit_RESET);

	//OD SCL LOW
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SCL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	//	GPIO_PCF8574_I2C_InitStruct.Pull = GPIO_PULLDOWN;
        
	GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);
        
	GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL, Bit_RESET);
        
	while (GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL) != Bit_RESET);

	//OD SDA HIGH
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SDA;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	//	GPIO_PCF8574_I2C_InitStruct.Pull = GPIO_PULLUP;

	GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);

	GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA, Bit_SET);

	while (GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SDA) != Bit_SET);

	//OD SCL HIGH
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SCL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	//GPIO_I2CInitStructure.Pull = GPIO_PULLUP;

	GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);

	GPIO_WriteBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL, Bit_SET);

	while (GPIO_ReadInputDataBit(PCF8574_GPIO_I2C, PCF8574_PIN_SCL) != Bit_SET);

	// SETTING I2C
	RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);
        
        RCC_AHBPeriphClockCmd(PCF8574_RCC_SDA, ENABLE);
	RCC_APB1PeriphClockCmd(PCF8574_RCC_APB_I2C, ENABLE);
        
        
	GPIO_PCF8574_I2C_InitStruct.GPIO_Pin = PCF8574_PIN_SCL | PCF8574_PIN_SDA;
	GPIO_PCF8574_I2C_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_PCF8574_I2C_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_PCF8574_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PCF8574_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
        
	GPIO_Init(PCF8574_GPIO_I2C, &GPIO_PCF8574_I2C_InitStruct);

	//hi2c1.Instance->CR1 |= I2C_CR1_SWRST;
	//hi2c1.Instance->CR1 &= ~I2C_CR1_SWRST;

	I2C_Cmd(PCF8574_I2C_CH, ENABLE);
}

/****************************************************************
*FUNCTION NAME:selectPin
*FUNCTION     :selectPin
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
uint8_t HAL_PCF8574::selectPin(uint8_t pin)
{
	uchar temp;

	switch (pin)
	{
	case 0:
		temp = PCF8574_MASK_PIN0;
		break;
	case 1:
		temp = PCF8574_MASK_PIN1;
		break;
	case 2:
		temp = PCF8574_MASK_PIN2;
		break;
	case 3:
		temp = PCF8574_MASK_PIN3;
		break;
	case 4:
		temp = PCF8574_MASK_PIN4;
		break;
	case 5:
		temp = PCF8574_MASK_PIN5;
		break;
	case 6:
		temp = PCF8574_MASK_PIN6;
		break;
	case 7:
		temp = PCF8574_MASK_PIN7;
		break;
	default:
		temp = 0;
		break;
	};

	return temp;
}


/****************************************************************
*FUNCTION NAME:writePin
*FUNCTION     :writePin
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
void HAL_PCF8574::writePin(uint8_t address, uint8_t pin, uint8_t val)
{
	uint8_t temp;
	uint8_t data;

	temp = selectPin(pin);

	if (val == 1)
		data = (0xFF)&temp;
	else
		data = (0x00)&temp;

	writeRegister(address, data);

}
/****************************************************************
*FUNCTION NAME:readPin
*FUNCTION     :readPin
*INPUT        :address, register, value
*OUTPUT       :none
****************************************************************/
bool HAL_PCF8574::readPin(uint8_t address, uint8_t pin, uint8_t val)
{
	uint8_t temp1, data;

	temp1 = selectPin(pin);

	data = readRegister(address, val);

	data = data & temp1 ;

	if(data)
          return 1;
        else
          return 0;
}

// uint8_t HAL_PCF8574::read(const uint8_t pin)
// {
//     if (pin > 7)
//     {
//         _error = PCF8574_PIN_ERROR;
//         return 0;
//     }
//     HAL_PCF8574::read8();
//     return (_dataIn & (1 << pin)) > 0;
// }

// void HAL_PCF8574::toggle(const uint8_t pin)
// {
//     if (pin > 7)
//     {
//         _error = PCF8574_PIN_ERROR;
//         return;
//     }
//     toggleMask(1 << pin);
// }

// void HAL_PCF8574::toggleMask(const uint8_t mask)
// {
//     _dataOut ^= mask;
//     HAL_PCF8574::write8(_dataOut);
// }

// int HAL_PCF8574::lastError()
// {
//     int e = _error;
//     _error = PCF8574_OK;
//     return e;
// }

// //added 0.1.07/08 Septillion
// uint8_t HAL_PCF8574::readButton8(const uint8_t mask)
// {
//     uint8_t temp = _dataOut;
//     HAL_PCF8574::write8(mask | _dataOut); 
//     HAL_PCF8574::read8();
//     HAL_PCF8574::write8(temp);
//     return _dataIn;
// }

// //added 0.1.07 Septillion
// uint8_t HAL_PCF8574::readButton(const uint8_t pin)
// {
//     if (pin > 7)
//     {
//         _error = PCF8574_PIN_ERROR;
//         return 0;
//     }
//     uint8_t temp = _dataOut;
//     HAL_PCF8574::write(pin, HIGH);
//     uint8_t rtn = HAL_PCF8574::read(pin);
//     HAL_PCF8574::write8(temp);
//     return rtn;
// }

// // -- END OF FILE --



#endif

#endif