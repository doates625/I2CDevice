# I2CDevice
Class for communicating with I2C generic devices  
Written by Dan Oates (WPI Class of 2020)

### Description
This class acts as an abstraction wrapper for communicating with I2C devices with support for both the Arduino and Mbed platforms. The class takes an I2C bus, I2C address, and endian order on construction, and has template methods for reading and writing integer types.
  
This class supports sequential register reads via the get_seq() method. The macro I2CDEVICE_BUFFER_SIZE must be set, which is the maximum number of bytes readable by read_seq. Because this class supports reading up to 64-bit (8-byte) integers, the macro must be defined as 8 or more. If the macro is not defined or defined as less than 8, this header file throws an error at compile time.

### Dependencies
- [Platform](https://github.com/doates625/Platform.git)
- [Struct](https://github.com/doates625/Struct.git)

### References
- [Arduino I2C](https://www.arduino.cc/en/reference/wire)
- [Mbed I2C](https://os.mbed.com/docs/mbed-os/v5.14/apis/i2c.html)