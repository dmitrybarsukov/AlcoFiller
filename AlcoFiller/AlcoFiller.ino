#include "Pinout.h"

#include <Servo.h>

const int PIN_SERVO = Pins::SERVO_CONTROL;
const int PIN_ANALOG = A7;

Servo myServo;

/*
 Name:		AlcoFiller.ino
 Created:	12/11/2019 12:34:46 AM
 Author:	Dmitry Barsukov
*/

// the setup function runs once when you press reset or power the board
void setup() {
	myServo.attach(PIN_SERVO);
	Serial.begin(115200);
	pinMode(PIN_ANALOG, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	myServo.write(90);
	Serial.println(analogRead(PIN_ANALOG));
	delay(20);
}
