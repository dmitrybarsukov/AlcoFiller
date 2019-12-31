#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define HOLD_TIME 800

namespace Periph
{
	class Button
	{
	public:
		Button(byte pin, boolean pullup)
		{
			_pin = pin;
			pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
			pushedAt = 0x7FFFFFFF;
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

		boolean isHold()
		{
			if (millis() > pushedAt + HOLD_TIME)
			{
				pushedAt = 0x7FFFFFFF;
				return true;
			}
			return false;
		}

		void process()
		{
			_prevState = _currState;
			_currState = !digitalRead(_pin);
			if (isPushed())
				pushedAt = millis();
			else if (isReleased())
				pushedAt = 0x7FFFFFFF;
		}

	private:
		byte _pin;
		int _currState;
		int _prevState;
		unsigned long pushedAt;
	};
}

#endif // BUTTON_H
