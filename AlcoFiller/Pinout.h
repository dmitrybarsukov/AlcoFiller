#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

namespace Pins
{
	const unsigned char TM1637_SCL = A5;
	const unsigned char TM1637_SDA = A4;

	const unsigned char SERVO_FEEDBACK = A0;
	const unsigned char SERVO_CONTROL = 3;
}

#endif // PINOUT_H