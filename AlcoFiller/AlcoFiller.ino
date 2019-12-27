/*
 Name:		AlcoFiller.ino
 Created:	12/11/2019 12:34:46 AM
 Author:	Dmitry Barsukov
*/

#include <known_16bit_timers.h>
#include <Adafruit_TiCoServo.h>
#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include "Pinout.h"
#include "Button.h"
#include "Encoder.h"

Periph::Button mainButton(Pins::BUTTON_BIG, false);
Periph::Button encButton(Pins::ENCODER_BUTTON, false);
Periph::Button caseOpenDetector(Pins::CASE_OPEN_DETECTOR, true);
Periph::Encoder encoder(Pins::ENCODER_S1, Pins::ENCODER_S2, false);
Adafruit_TiCoServo servo;
Adafruit_NeoPixel pixels(4, Pins::RGB_LED_CONTROL);
TM1637Display display(Pins::TM1637_SCL, Pins::TM1637_SDA);

byte display_Err[] = { 0x79, 0x50, 0x50, 0x00 };
byte display_CAL[] = { 0x39, 0x77, 0x38 };
byte display_A[] = { 0x77 };
byte display_P[] = { 0x73 };

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	pixels.begin();
	display.setBrightness(7, true);
	servo.attach(Pins::SERVO_CONTROL);
}

// the loop function runs over and over again until power down or reset
int cnt = 0;
void loop() {
	caseOpenDetector.process();
	encoder.process();
	if (encoder.isRotatedLeft())
		cnt = cnt == 0 ? 3 : cnt - 1;
	else if (encoder.isRotatedRight())
		cnt = cnt == 3 ? 0 : cnt + 1;
	pixels.fill(0);
	pixels.setPixelColor(cnt, 0xFF0000);
	pixels.show();
	if(caseOpenDetector.isDown())
		servo.write(180 - cnt * 45);
	Serial.println(analogRead(Pins::SERVO_FEEDBACK));
}
