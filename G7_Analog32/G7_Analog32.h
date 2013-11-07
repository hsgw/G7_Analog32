#ifndef G7_ANALOG32_H
#define G7_ANALOG32_H

#include "Arduino.h"

#define AIN_A 0 
#define AIN_B 1

class G7_Analog32{
public:
	G7_Analog32(uint8_t _addr = 0);

	void init(void);

	int16_t readRawValue(uint8_t _ic, uint8_t _pin);
	int16_t readVoltage(uint8_t _ic, uint8_t _pin);

	int16_t analogRead(uint8_t _ic, uint8_t _pin);

	void setMaxVoltage(uint16_t _mv);
	uint16_t getMaxVoltage();

private:
	static const uint8_t MUX_ADDR[16];
	static const uint8_t ADC_CONFIG_HIGH[2];

	uint8_t dio_addr;
	uint8_t adc_addr;
	uint8_t currentMuxAddr;

	uint16_t maxVoltage;

	inline void i2cSendByte(uint8_t _addr, uint8_t _cmd, uint8_t _data);
	inline void i2cSend2Bytes(uint8_t _addr, uint8_t _cmd, uint8_t _data_h, uint8_t _data_l);

	inline uint8_t i2cReadByte(uint8_t _addr, uint8_t _cmd);
	inline int16_t i2cRead2Bytes(uint8_t _addr, uint8_t _cmd);
};

#endif