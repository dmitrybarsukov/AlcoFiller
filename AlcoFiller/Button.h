#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace Periph
{
	class Button
	{
	public:
		Button(byte pin, boolean pullup)
		{
			_pin = pin;
			pinMode(pin, pullup ? INPUT_PULLUP : INPUT);

		}

		boolean isDown()
		{
			return _currState;
		}

		boolean isPushed()
		{
			return _currState && !_prevState;
		}

		boolean isReleased()
		{
			return !_currState && _prevState;
		}

		void process()
		{
			_prevState = _currState;
			_currState = !digitalRead(_pin);
		}

	private:
		byte _pin;
		int _currState;
		int _prevState;
	};
}

#endif // BUTTON_H
