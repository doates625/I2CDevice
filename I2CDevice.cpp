/**
 * @file I2CDevice.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include "I2CDevice.h"

/**
 * @brief Constructor for I2C device
 * @param i2c I2C interface pointer
 * @param i2c_addr I2C address of device
 * @param endian Little-endian or Big-endian
 */
I2CDevice::I2CDevice(i2c_t* i2c, uint8_t i2c_addr, Struct::endian_t endian) :
	str(buffer, endian)
{
	this->i2c = i2c;
	#if defined(PLATFORM_ARDUINO)
		this->i2c_addr = i2c_addr;
	#elif defined(PLATFORM_MBED)
		this->i2c_addr = i2c_addr << 1;
	#endif
}

/**
 * @brief Returns reference to internal I2C bus interface
 */
I2CDevice::i2c_t& I2CDevice::get_i2c()
{
	return (*i2c);
}

/**
 * @brief Gets sequence of registers from device
 * @param reg_addr Starting register address
 * @param num_bytes Number of bytes to read
 * @return Reference to this
 */
I2CDevice& I2CDevice::get_seq(uint8_t reg_addr, uint8_t num_bytes)
{
	#if defined(PLATFORM_ARDUINO)
		i2c->beginTransmission(i2c_addr);
		i2c->write(reg_addr);
		i2c->endTransmission(false);
		i2c->requestFrom(i2c_addr, num_bytes, (uint8_t)1);
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			buffer[i] = i2c->read();
		}
		str.reset();
	#elif defined(PLATFORM_MBED)
		str.reset() << reg_addr;
		i2c->write(i2c_addr, buffer, 1, true);
		i2c->read(i2c_addr, buffer, num_bytes);
	#endif
	return (*this);
}