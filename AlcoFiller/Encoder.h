#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

namespace Periph
{
	class Encoder
	{
	public:
		Encoder(byte pinA, byte pinB, boolean pullup)
		{
			_pinA = pinA;
			_pinB = pinB;
			pinMode(_pinA, pullup ? INPUT_PULLUP : INPUT);
			pinMode(_pinB, pullup ? INPUT_PULLUP : INPUT);
			stateHist = 0;
		}

		boolean isRotatedRight()
		{
			boolean res = stateHist == 0x1E;
			if (res)
				stateHist = 0;
			return res;
		}

		boolean isRotatedLeft()
		{
			boolean res = stateHist == 0x2D;
			if (res)
				stateHist = 0;
			return res;
		}

		void process()
		{
			byte state = (!digitalRead(_pinA)) | ((!digitalRead(_pinB)) << 1);
			if (state != (stateHist & 0x3))
				stateHist = (stateHist << 2) | state;
		}

	private:
		byte _pinA;
		byte _pinB;
		byte stateHist;
	};

}


#endif // ENCODER_H
