#ifndef SERVOFB_H
#define SERVOFB_H

#include <Arduino.h>
#include <known_16bit_timers.h>
#include <Adafruit_TiCoServo.h>

namespace Periph
{
	class ServoFb
	{
	public:
		ServoFb(byte controlPin, byte feedbackPin, int maxAngleError = 5)
		{
			_servo.attach(controlPin, 500, 2400);
			_fb = AnalogIn(feedbackPin);
			_maxFbError = maxAngleError * 3.2055;
		}

		void rotateTo(int angle)
		{
			angle = constrain(angle, 0, 180);
			_targetFbValue = angleToFb(angle);
			Serial.print("TO = ");
			Serial.println(angle);
			_servo.write(angle);
		}

		boolean isInTarget()
		{
			Serial.println(_fb.getValue());
			int dlt = _fb.getValue() - _targetFbValue;
			return abs(dlt) <= _maxFbError;
		}

	private:
		Adafruit_TiCoServo _servo;
		AnalogIn _fb = AnalogIn(0);
		int _maxFbError;
		int _targetFbValue;

		int angleToFb(int angle)
		{
			int fb = angle * 3.2055 + 83.341;
			return constrain(fb, 0, 1023);
		}

		int fbToAngle(int fbVal)
		{
			int val = fbVal * 0.3119 - 25.992;
			return constrain(val, 0, 180);
		}
	};
}

#endif // SERVOFB_H
