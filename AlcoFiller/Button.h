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
			return currState;
		}

		boolean isPushed()
		{
			return currState && !prevState;
		}

		boolean isReleased()
		{
			return !currState && prevState;
		}

		void process()
		{
			prevState = currState;
			currState = !digitalRead(_pin);
		}

	private:
		byte _pin;
		int currState;
		int prevState;
	};

}


#endif // BUTTON_H
