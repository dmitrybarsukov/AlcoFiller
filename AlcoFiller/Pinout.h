#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

namespace Pins
{
	const byte TM1637_SCL = A5;
	const byte TM1637_SDA = A4;

	const byte SERVO_FEEDBACK = A0;
	const byte SERVO_CONTROL = 9;

	const byte PUMP_CONTROL = 10;

	const byte IR_LED_CONTROL = 13;
	const byte IR_RCV_INPUT1 = A1;
	const byte IR_RCV_INPUT2 = A7;
	const byte IR_RCV_INPUT3 = A6;
	const byte IR_RCV_INPUT4 = A3;

	const byte BUTTON_BIG = 12;
	const byte CASE_OPEN_DETECTOR = 8;
	const byte ENCODER_BUTTON = 4;
	const byte ENCODER_S1 = 5;
	const byte ENCODER_S2 = 6;

	const byte RGB_LED_CONTROL = 7;
}

#endif // PINOUT_H