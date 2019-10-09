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
I2CDevice::I2CDevice(I2CDEVICE_I2C_CLASS* i2c, uint8_t i2c_addr, endian_t endian)
{
	this->i2c = i2c;
#if defined(PLATFORM_ARDUINO)
	this->i2c_addr = i2c_addr;
#elif defined(PLATFORM_MBED)
	this->i2c_addr = i2c_addr << 1;
	this->buffer_idx = 0;
	this->buffer_len = 0;
#endif
	this->endian = endian;
}

/**
 * @brief Empty constructor for I2C device (for declaration only)
 */
I2CDevice::I2CDevice()
{
	this->i2c = 0;
	this->i2c_addr = 0x00;
#if defined(PLATFORM_MBED)
	this->buffer_idx = 0;
	this->buffer_len = 0;
#endif
	this->endian = lsb_first;
}

/**
 * @brief Writes int8_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_int8(uint8_t reg_addr, int8_t value)
{
	union64.int8s[0] = value;
	write_bytes(reg_addr, 1);
}

/**
 * @brief Writes int16_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_int16(uint8_t reg_addr, int16_t value)
{
	union64.int16s[0] = value;
	write_bytes(reg_addr, 2);
}

/**
 * @brief Writes int32_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_int32(uint8_t reg_addr, int32_t value)
{
	union64.int32s[0] = value;
	write_bytes(reg_addr, 4);
}

/**
 * @brief Writes int64_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_int64(uint8_t reg_addr, int64_t value)
{
	union64.int64 = value;
	write_bytes(reg_addr, 8);
}

/**
 * @brief Writes uint8_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_uint8(uint8_t reg_addr, uint8_t value)
{
	union64.uint8s[0] = value;
	write_bytes(reg_addr, 1);
}

/**
 * @brief Writes uint16_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_uint16(uint8_t reg_addr, uint16_t value)
{
	union64.uint16s[0] = value;
	write_bytes(reg_addr, 2);
}

/**
 * @brief Writes uint32_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_uint32(uint8_t reg_addr, uint32_t value)
{
	union64.uint32s[0] = value;
	write_bytes(reg_addr, 4);
}

/**
 * @brief Writes uint64_t to device register
 * @param reg_addr Register address to write to
 * @param value Value to write
 */
void I2CDevice::write_uint64(uint8_t reg_addr, uint64_t value)
{
	union64.uint64 = value;
	write_bytes(reg_addr, 8);
}

/**
 * @brief Reads int8_t from given device register
 * @param reg_addr Register address to read from
 */
int8_t I2CDevice::read_int8(uint8_t reg_addr)
{
	read_sequence(reg_addr, 1);
	return read_int8();
}

/**
 * @brief Reads int16_t from given device register
 * @param reg_addr Register address to read from
 */
int16_t I2CDevice::read_int16(uint8_t reg_addr)
{
	read_sequence(reg_addr, 2);
	return read_int16();
}

/**
 * @brief Reads int32_t from given device register
 * @param reg_addr Register address to read from
 */
int32_t I2CDevice::read_int32(uint8_t reg_addr)
{
	read_sequence(reg_addr, 4);
	return read_int32();
}

/**
 * @brief Reads int64_t from given device register
 * @param reg_addr Register address to read from
 */
int64_t I2CDevice::read_int64(uint8_t reg_addr)
{
	read_sequence(reg_addr, 8);
	return read_int64();
}

/**
 * @brief Reads uint8_t from given device register
 * @param reg_addr Register address to read from
 */
uint8_t I2CDevice::read_uint8(uint8_t reg_addr)
{
	read_sequence(reg_addr, 1);
	return read_uint8();
}

/**
 * @brief Reads uint16_t from given device register
 * @param reg_addr Register address to read from
 */
uint16_t I2CDevice::read_uint16(uint8_t reg_addr)
{
	read_sequence(reg_addr, 2);
	return read_uint16();
}

/**
 * @brief Reads uint32_t from given device register
 * @param reg_addr Register address to read from
 */
uint32_t I2CDevice::read_uint32(uint8_t reg_addr)
{
	read_sequence(reg_addr, 4);
	return read_uint32();
}

/**
 * @brief Reads uint64_t from given device register
 * @param reg_addr Register address to read from
 */
uint64_t I2CDevice::read_uint64(uint8_t reg_addr)
{
	read_sequence(reg_addr, 8);
	return read_uint64();
}

/**
 * @brief Initiates sequential register read in I2C device
 * @param reg_addr Starting register address
 * @param num_bytes Number of bytes to read starting from reg_addr
 */
void I2CDevice::read_sequence(uint8_t reg_addr, uint8_t num_bytes)
{
#if defined(PLATFORM_ARDUINO)
	i2c->beginTransmission(i2c_addr);
	i2c->write(reg_addr);
	i2c->endTransmission(false);
	const uint8_t send_stop = 1;
	i2c->requestFrom(i2c_addr, num_bytes, send_stop);
#elif defined(PLATFORM_MBED)
	buffer[0] = reg_addr;
	i2c->write(i2c_addr, buffer, 1, true);
	i2c->read(i2c_addr, buffer, num_bytes);
	buffer_idx = 0;
	buffer_len = num_bytes;
#endif
}

/**
 * @brief Reads int8_t from device after call to read_sequence
 */
int8_t I2CDevice::read_int8()
{
	read_bytes(1);
	return union64.int8s[0];
}

/**
 * @brief Reads int16_t from device after call to read_sequence
 */
int16_t I2CDevice::read_int16()
{
	read_bytes(2);
	return union64.int16s[0];
}

/**
 * @brief Reads int32_t from device after call to read_sequence
 */
int32_t I2CDevice::read_int32()
{
	read_bytes(4);
	return union64.int32s[0];
}

/**
 * @brief Reads int64_t from device after call to read_sequence
 */
int64_t I2CDevice::read_int64()
{
	read_bytes(8);
	return union64.int64;
}

/**
 * @brief Reads uint8_t from device after call to read_sequence
 */
uint8_t I2CDevice::read_uint8()
{
	read_bytes(1);
	return union64.uint8s[0];
}

/**
 * @brief Reads uint16_t from device after call to read_sequence
 */
uint16_t I2CDevice::read_uint16()
{
	read_bytes(2);
	return union64.uint16s[0];
}

/**
 * @brief Reads uint32_t from device after call to read_sequence
 */
uint32_t I2CDevice::read_uint32()
{
	read_bytes(4);
	return union64.uint32s[0];
}

/**
 * @brief Reads uint64_t from device after call to read_sequence
 */
uint64_t I2CDevice::read_uint64()
{
	read_bytes(8);
	return union64.uint64;
}

/**
 * @brief Writes bytes from internal union to device register
 * @param reg_addr Device register to write to
 * @param num_bytes Length of byte buffer
 */
void I2CDevice::write_bytes(uint8_t reg_addr, uint8_t num_bytes)
{
#if defined(PLATFORM_ARDUINO)

	// Begin write to register address
	i2c->beginTransmission(i2c_addr);
	i2c->write(reg_addr);

	// Check endian order
	if (endian == lsb_first)
	{
		// LSB first - write in ascending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			i2c->write(union64.bytes[i]);
		}
	}
	else
	{
		// MSB first - write in descending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			i2c->write(union64.bytes[num_bytes - 1 - i]);
		}
	}

	// End write and free the bus
	i2c->endTransmission(true);

#elif defined(PLATFORM_MBED)

	
	// Write register address first
	buffer[0] = reg_addr;

	// Check endian order
	if (endian == lsb_first)
	{
		// LSB first - write in ascending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			buffer[i + 1] = union64.bytes[i];
		}
	}
	else
	{
		// MSB first - write in descending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			buffer[i + 1] = union64.bytes[num_bytes - 1 - i];
		}
	}

	// Write buffer data with address
	i2c->write(i2c_addr, buffer, num_bytes + 1);

#endif
}

/**
 * @brief Reads bytes into internal buffer after call to read_sequence.
 * @param num_bytes Length of byte buffer
 */
void I2CDevice::read_bytes(uint8_t num_bytes)
{
#if defined(PLATFORM_ARDUINO)

	// Check endian order
	if (endian == lsb_first)
	{
		// LSB first - read in ascending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			union64.bytes[i] = i2c->read();
		}
	}
	else
	{
		// MSB first - read in descending order
		for (uint8_t i = num_bytes - 1; i < 255; i--)
		{
			union64.bytes[i] = i2c->read();
		}
	}

#elif defined(PLATFORM_MBED)

	// Get buffer endpoint for this read
	uint8_t buffer_end = buffer_idx + num_bytes;

	// Check endian order
	if (endian == lsb_first)
	{
		// LSB first - read in ascending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			union64.bytes[i] = buffer[i + buffer_idx];
		}
	}
	else
	{
		// MSB first - read in descending order
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			union64.bytes[i] = buffer[buffer_end - 1 - i];
		}
	}

	// Update buffer index
	if (buffer_end < buffer_len)
	{
		buffer_idx = buffer_end;
	}
	else
	{
		buffer_idx = 0;
	}

#endif
}