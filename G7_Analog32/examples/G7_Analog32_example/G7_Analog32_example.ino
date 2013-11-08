/*==================================================================
	G7_Analog32 example
==================================================================*/

#include <Wire.h>
#include "G7_Analog32.h"

// 基板上のADDR_SELを1にした場合は
// G7_Digital64 adc(1)とする。
G7_Analog32 adc;

void setup() {
	Serial.begin(9600);

	// I2Cの初期化
	Wire.begin();

	// G7_Analog32の初期化
	adc.init();

	// analogRead()のために実測した電圧の最大値を設定
	adc.setMaxVoltage(4997);
}

void loop() {
	Serial.println(" AIN_A :");
	for(int i=0;i<16;i++){
		Serial.print(i);
		Serial.print(": ");

		// AIN_Aの各ピンの電圧をミリボルト(1V = 1000mV)で取得
		Serial.print(adc.readVoltage(AIN_A,i));
		Serial.print("mV: ");
		// AIN_Aの各ピンの電圧をmaxVoltageに設定された値を最大値として１０ビットで取得
		Serial.print(adc.analogRead(AIN_A,i));
		Serial.print(" / ");
	}
	Serial.println();
	Serial.println("***********************************");
	delay(1000);
}