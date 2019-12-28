#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>

namespace Periph
{
	class Output
	{
	public:
		Output(byte pin, boolean state = false, boolean inverted = false)
		{
			_pin = pin;
			_inverted = inverted;
			pinMode(_pin, OUTPUT);
			set(state);
		}

		void on()
		{
			set(true);
		}

		void off()
		{
			set(false);
		}

		void toggle()
		{
			set(!_state);
		}

		boolean getState()
		{
			return _state;
		}

		void set(boolean state)
		{
			digitalWrite(_pin, _inverted ^ (_state = state));
		}

	private:
		byte _pin;
		boolean _inverted;
		boolean _state;
	};
}

#endif // OUTPUT_H
