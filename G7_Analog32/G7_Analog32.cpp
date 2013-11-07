#include "G7_Analog32.h"
#include "Arduino.h"
#include "Wire.h"

//Address
#define PCA9538_ADDR (0x70)
#define ADS1015_ADDR (0x48)

//Command
//IO expander
#define PCA9538_REG_OUTPUT (1)
#define PCA9538_REG_CONFIG (3)

// ADC
#define ADS1015_REG_RESULT    (0x00) // read only
#define ADS1015_REG_CONFIG    (0x01) // 2bytes
#define ADS1015_REG_LO_THRESH (0x02) // 2bytes
#define ADS1015_REG_HI_THRESH (0x03) // 2bytes

// config register
#define ADS1015_CONFIG_HIGH_DEFAULT (B01000001)

#define ADS1015_CONFIG_HIGH_AIN0 (B11000001)
#define ADS1015_CONFIG_HIGH_AIN1 (B11010001)

#define ADS1015_CONFIG_LOW		 (B10010011)

// convert raw value to voltage (mV)
#define ADC_RESULT_VOLTAGE_GAIN (3)

const uint8_t G7_Analog32::MUX_ADDR[16] = {
	B00000000,B10000000,B01000000,B11000000,
	B00100000,B10100000,B01100000,B11100000,
	B00010000,B10010000,B01010000,B11010000,
	B00110000,B10110000,B01110000,B11110000
};

const uint8_t G7_Analog32::ADC_CONFIG_HIGH[2] = {
	ADS1015_CONFIG_HIGH_AIN1,ADS1015_CONFIG_HIGH_AIN0
};


G7_Analog32::G7_Analog32(uint8_t _addr){
	dio_addr = _addr + PCA9538_ADDR;
	adc_addr = _addr + ADS1015_ADDR;
	maxVoltage = 5000;
};

// private methods
inline void G7_Analog32::i2cSendByte(uint8_t _addr, uint8_t _cmd, uint8_t _data){
	Wire.beginTransmission(_addr);
	Wire.write(_cmd);
	Wire.write(_data);
	Wire.endTransmission();
}

inline void G7_Analog32::i2cSend2Bytes(uint8_t _addr, uint8_t _cmd, uint8_t _data_h, uint8_t _data_l){
	Wire.beginTransmission(_addr);
	Wire.write(_cmd);
	Wire.write(_data_h);
	Wire.write(_data_l);
	Wire.endTransmission();
}

inline uint8_t G7_Analog32::i2cReadByte(uint8_t _addr, uint8_t _cmd){
	uint8_t ret = NULL;
	Wire.beginTransmission(_addr);
	Wire.write(_cmd);
	Wire.endTransmission();
	Wire.requestFrom(_addr, (uint8_t)1);
	if(Wire.available()){
		ret = Wire.read();
	}
	return ret;
}

inline int16_t G7_Analog32::i2cRead2Bytes(uint8_t _addr, uint8_t _cmd){
	int16_t ret = -1;
	Wire.beginTransmission(_addr);
	Wire.write(_cmd);
	Wire.endTransmission();
	Wire.requestFrom(_addr, (uint8_t)2);
	if(Wire.available()){
		ret = (Wire.read() << 8) | (Wire.read());
	}
	return ret;
}

// public methods

void G7_Analog32::init(void){
	currentMuxAddr = 0;
	//set MUX address 0
	i2cSendByte(dio_addr, PCA9538_REG_OUTPUT, 0x00);
	//set IO expander all output
	i2cSendByte(dio_addr, PCA9538_REG_CONFIG, 0x00);
	delay(10);

	//set ADC standby Mode
	i2cSend2Bytes(adc_addr, ADS1015_REG_CONFIG, ADS1015_CONFIG_HIGH_DEFAULT, ADS1015_CONFIG_LOW);
}

int16_t G7_Analog32::readRawValue(uint8_t _ic, uint8_t _pin){
	// select mux
	currentMuxAddr = (currentMuxAddr & (0x0F << _ic)) | (MUX_ADDR[_pin] >> _ic);
	//Serial.println(currentMuxAddr,BIN);
	i2cSendByte(dio_addr, PCA9538_REG_OUTPUT, currentMuxAddr);
	delay(1);
	// select adc mux and start convert
	i2cSend2Bytes(adc_addr, ADS1015_REG_CONFIG, ADC_CONFIG_HIGH[_ic], ADS1015_CONFIG_LOW);
	//Serial.println((uint16_t)i2cRead2Bytes(adc_addr, ADS1015_REG_CONFIG),BIN);
	delay(1);
	//Serial.println((uint16_t)i2cRead2Bytes(adc_addr, ADS1015_REG_CONFIG),BIN);
	return i2cRead2Bytes(adc_addr, ADS1015_REG_RESULT) >> 4;
}

int16_t G7_Analog32::readVoltage(uint8_t _ic, uint8_t _pin){
	return readRawValue(_ic, _pin) * ADC_RESULT_VOLTAGE_GAIN;
}

int16_t G7_Analog32::analogRead(uint8_t _ic, uint8_t _pin){
	return (int16_t)((float)readVoltage(_ic, _pin)/(float)maxVoltage*1024.0);
}

void G7_Analog32::setMaxVoltage(uint16_t _mv){
	maxVoltage = _mv;
}

uint16_t G7_Analog32::getMaxVoltage(){
	return maxVoltage;
}
