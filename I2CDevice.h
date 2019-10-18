/**
 * @file I2CDevice.h
 * @brief Class for communicating with generic I2C devices
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once
#include <Platform.h>
#include <Unions.h>

/**
 * Platform-specific I2C Interface Class
 */
#if defined(PLATFORM_ARDUINO)
	#include <Wire.h>
	#define I2CDEVICE_I2C_CLASS TwoWire
#elif defined(PLATFORM_MBED)
	#define I2CDEVICE_I2C_CLASS I2C
	#if !defined(I2CDEVICE_BUFFER_SIZE)
		#warning I2CDEVICE_BUFFER_SIZE must be defined. Defining as 8...
		#define I2CDEVICE_BUFFER_SIZE 8
	#elif I2CDEVICE_BUFFER_SIZE < 8
		#warning I2CDevice requires I2CDEVICE_BUFFER_SIZE >= 8. Setting to 8...
		#undef I2CDEVICE_BUFFER_SIZE
		#define I2CDEVICE_BUFFER_SIZE 8
	#endif
#endif

/**
 * Class Declaration
 */
class I2CDevice
{
public:

	// Endian enum
	typedef enum
	{
		msb_first,
		lsb_first,
	}
	endian_t;

	// Constructors and Basics
	I2CDevice(I2CDEVICE_I2C_CLASS* i2c, uint8_t i2c_addr, endian_t endian);
	I2CDevice();
	I2CDEVICE_I2C_CLASS& get_i2c();

	// Write Methods
	void write_int8(uint8_t reg_addr, int8_t value);
	void write_int16(uint8_t reg_addr, int16_t value);
	void write_int32(uint8_t reg_addr, int32_t value);
	void write_int64(uint8_t reg_addr, int64_t value);
	void write_uint8(uint8_t reg_addr, uint8_t value);
	void write_uint16(uint8_t reg_addr, uint16_t value);
	void write_uint32(uint8_t reg_addr, uint32_t value);
	void write_uint64(uint8_t reg_addr, uint64_t value);

	// Single read methods
	int8_t read_int8(uint8_t reg_addr);
	int16_t read_int16(uint8_t reg_addr);
	int32_t read_int32(uint8_t reg_addr);
	int64_t read_int64(uint8_t reg_addr);
	uint8_t read_uint8(uint8_t reg_addr);
	uint16_t read_uint16(uint8_t reg_addr);
	uint32_t read_uint32(uint8_t reg_addr);
	uint64_t read_uint64(uint8_t reg_addr);

	// Sequential read methods
	void read_sequence(uint8_t reg_addr, uint8_t num_bytes);
	int8_t read_int8();
	int16_t read_int16();
	int32_t read_int32();
	int64_t read_int64();
	uint8_t read_uint8();
	uint16_t read_uint16();
	uint32_t read_uint32();
	uint64_t read_uint64();

protected:

	// Device information
	I2CDEVICE_I2C_CLASS* i2c;
	uint8_t i2c_addr;
	endian_t endian;

	// Data storage
	union64_t union64;
#if defined(PLATFORM_MBED)
	uint8_t buffer_idx;
	uint8_t buffer_len;
	char buffer[I2CDEVICE_BUFFER_SIZE + 1];
#endif

	// Read-write methods
	void write_bytes(uint8_t reg_addr, uint8_t num_bytes);
	void read_bytes(uint8_t num_bytes);
};