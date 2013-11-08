/**
* @file G7_Analog32.h
* @author Takuya Urakawa (galileo-7.com)
* @license MIT Licence
*
* @brief This is a library for Galileo-7 I2C 32ch Analog-Inputs "OOMORI" Shield.
* 	     I2C 32ch Analog-Digital Converter.
*
* @section HISTORY
* v1.0 - First release
*/

/*!
 * @mainpage
 * 
 * ガリレオセブンにて販売中のI2C 32ch Analog-Inputs "大盛" ShieldのArduino用ライブラリです。<br
 * I2Cで通信し、32chのアナログ入力を追加します。<br>
 * <br>
 * 詳しくは以下クラスのリファレンスを参照して下さい。
 * - G7_Analog32
*/

#ifndef G7_ANALOG32_H
#define G7_ANALOG32_H

#include "Arduino.h"

/*===========================================================================*/
//! IC番号の代りに使用する定数
#define AIN_A 0 
//! IC番号の代りに使用する定数
#define AIN_B 1

/*===========================================================================*/

//! 32chのアナログ入力を追加します
//! @note このシールドを2枚使用したいときは基板上のADDR_SELで選択するアドレスをそれぞれ変えること
class G7_Analog32{
public:
	//! コンストラクタ
	//! @param _addr 基板上のADDR_SELで選択したアドレス(0か1)
	//! 初期値は0
	G7_Analog32(uint8_t _addr = 0);

	//! 初期化する
	void init(void);

	//! 指定したピンの電圧を取得します
	/** @return 6.144Vを最大値とした11bit(0～2047)の値を返す
	  * @note 電源電圧を基準としていません
	  * @param _ic IC番号(AIN_A / AIN_B)
	  * @param _pin ピン番号(0-16)
	*/
	int16_t readRawValue(uint8_t _ic, uint8_t _pin);

	//! 指定したピンの電圧を取得します
	//! @return 電圧をmV（ミリボルト）で返す(1V = 1000mV)
	//! @param _ic IC番号(AIN_A / AIN_B)
	//! @param _pin ピン番号(0-16)
	int16_t readVoltage(uint8_t _ic, uint8_t _pin);

	//! 指定したピンの電圧を取得します
	//! @note Arduinoとの互換性のための関数です。setMaxVoltage()で実際の電圧値を指定して下さい。
	//! @return maxVoltageを最大値とした10bit(0～1024)の値を返す
	//! @param _ic IC番号(AIN_A / AIN_B)
	//! @param _pin ピン番号(0-16)
	int16_t analogRead(uint8_t _ic, uint8_t _pin);

	//! analogRead()で使用する電圧の最大値を指定します
	//! @param _mv 電圧の最大値（mV(ミリボルト)）
	void setMaxVoltage(uint16_t _mv);

	//! 現在指定されているmaxVoltageを返す
	//! @return 現在指定されているmaxVoltage(mV(ミリボルト))
	uint16_t getMaxVoltage();

private:
	static const uint8_t MUX_ADDR[16];

	uint8_t dio_addr;
	uint8_t adc_addr;
	uint8_t currentMuxAddr;

	uint16_t maxVoltage;

	//! コマンドとデータをi2cで送信する
	inline void i2cSendByte(uint8_t _addr, uint8_t _cmd, uint8_t _data);
	//! コマンドとデータ(2バイト)をi2cで送信する
	inline void i2cSend2Bytes(uint8_t _addr, uint8_t _cmd, uint8_t _data_h, uint8_t _data_l);

	//! i2cからデータを読み込む
	//! @return 成功したら受信した値、失敗した場合はNULL
	inline uint8_t i2cReadByte(uint8_t _addr, uint8_t _cmd);
	//! i2cからデータを2バイト読み込む
	//! @return 成功したら受信した値、失敗した場合は-1
	inline int16_t i2cRead2Bytes(uint8_t _addr, uint8_t _cmd);
};

#endif