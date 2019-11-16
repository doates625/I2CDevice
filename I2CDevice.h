/**
 * @file I2CDevice.h
 * @brief Class for communicating with generic I2C devices
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once
#include <Platform.h>
#include <Struct.h>

/**
 * I2C Input Buffer Size Macro
 */
#if !defined(I2CDEVICE_BUFFER_SIZE)
	#error I2CDEVICE_BUFFER_SIZE not defined
#elif I2CDEVICE_BUFFER_SIZE < 4
	#error I2CDEVICE_BUFFER_SIZE must be >= 8
#endif

/**
 * Class Declaration
 */
class I2CDevice
{
public:

	// I2C interface type
	#if defined(PLATFORM_ARDUINO)
		typedef TwoWire i2c_t;
	#elif defined(PLATFORM_MBED)
		typedef I2C i2c_t;
	#endif

	// Constructor and Basics
	I2CDevice(i2c_t* i2c, uint8_t i2c_addr, Struct::endian_t endian);
	i2c_t& get_i2c();

	// Getters and Setters
	template <typename T> I2CDevice& set(uint8_t reg_addr, T val);
	I2CDevice& get_seq(uint8_t reg_addr, uint8_t num_bytes);
	template <typename T> I2CDevice& get(T& val);
	template <typename T> I2CDevice& operator>>(T& val);
	template <typename T> operator T();
	template <typename T> I2CDevice& get(uint8_t reg_addr, T& val);

protected:
	i2c_t* i2c;
	uint8_t i2c_addr;
	uint8_t buffer[I2CDEVICE_BUFFER_SIZE + 1];
	Struct str;
private:
	I2CDevice(const I2CDevice&);
	I2CDevice& operator=(const I2CDevice&);
};

/**
 * Template Method Definitions
 */

/**
 * @brief Sets value of given register address
 * @param reg_addr Register address
 * @param val Value to write
 * @return Reference to this
 */
template <typename T>
I2CDevice& I2CDevice::set(uint8_t reg_addr, T val)
{
	// Pack binary data
	str.reset() << reg_addr << val;
	const uint8_t num_bytes = sizeof(val) + 1;

	// Write data to I2C
	#if defined(PLATFORM_ARDUINO)
		i2c->beginTransmission(i2c_addr);
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			i2c->write(buffer[i]);
		}
		i2c->endTransmission(true);
	#elif defined(PLATFORM_MBED)
		i2c->write(i2c_addr, buffer, num_bytes);
	#endif

	// Return self-reference
	return (*this);
}

/**
 * @brief Gets data after call to get_seq()
 * @param val Value to write into
 * @return Reference to this
 */
template <typename T>
I2CDevice& I2CDevice::get(T& val)
{
	str.get(val);
	return (*this);
}

/**
 * @brief Shorthand for get() method
 * @param val Value to write into
 * @return Reference to this
 */
template <typename T>
I2CDevice& I2CDevice::operator>>(T& val)
{
	return get(val);
}

/**
 * @brief Shorthand for get() method
 */
template <typename T>
I2CDevice::operator T()
{
	T val;
	get(val);
	return val;
}

/**
 * @brief Gets data from register
 * @param reg_addr Register address
 * @param val Value to write into
 * @return Reference to this
 */
template <typename T>
I2CDevice& I2CDevice::get(uint8_t reg_addr, T& val)
{
	get_seq(reg_addr, sizeof(val));
	return get(val);
}