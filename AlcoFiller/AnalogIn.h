#ifndef ANALOGIN_H
#define ANALOGIN_H

#include <Arduino.h>

namespace Periph
{
	class AnalogIn
	{
	public:
		AnalogIn(byte pin)
		{
			_pin = pin;
			pinMode(_pin, INPUT);
		}

		int getValue()
		{
			return analogRead(_pin);
		}

	private:
		byte _pin;
	};

}


#endif // ANALOGIN_H
