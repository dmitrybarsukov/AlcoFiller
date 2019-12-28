/*
 Name:		AlcoFiller.ino
 Created:	12/11/2019 12:34:46 AM
 Author:	Dmitry Barsukov
*/

#include <EEPROM.h>
#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TiCoServo.h>
#include "Pinout.h"
#include "Button.h"
#include "Encoder.h"
#include "Output.h"
#include "AnalogIn.h"
#include "Colors.h"

Periph::Button button(Pins::BUTTON_BIG, false);
Periph::Button encButton(Pins::ENCODER_BUTTON, false);
Periph::Button caseOpenDetector(Pins::CASE_OPEN_DETECTOR, true);
Periph::Encoder encoder(Pins::ENCODER_S2, Pins::ENCODER_S1, false);
Periph::Output pump(Pins::PUMP_CONTROL);
Periph::Output ledIR(Pins::IR_LED_CONTROL, false, true);
Periph::AnalogIn servoFb(Pins::SERVO_FEEDBACK);
Periph::AnalogIn rcvIR[] = { 
	Periph::AnalogIn(Pins::IR_RCV_INPUT1),
	Periph::AnalogIn(Pins::IR_RCV_INPUT2),
	Periph::AnalogIn(Pins::IR_RCV_INPUT3),
	Periph::AnalogIn(Pins::IR_RCV_INPUT4)
};
Adafruit_NeoPixel pixels(4, Pins::RGB_LED_CONTROL);
Adafruit_TiCoServo servo;
TM1637Display display(Pins::TM1637_SCL, Pins::TM1637_SDA);

int servoPos[] = { 0, 0, 0, 0 };

byte display_line[] = { 0x20, 0x20, 0x20, 0x20 };
byte display_Err[] = { 0x79, 0x50, 0x50, 0x00 };
byte display_CAL[] = { 0x39, 0x77, 0x38 };
byte display_A[] = { 0x77 };
byte display_P[] = { 0x73 };

namespace Mode
{
	typedef enum
	{
		Closed,
		Service,
		Auto,
		Manual
	} Mode;
}

Mode::Mode appMode;

void setup() {
	Serial.begin(115200);
	pixels.begin();
	servo.attach(Pins::SERVO_CONTROL);
	display.setBrightness(7, true);
	button.process();
	appMode = button.isDown() ? Mode::Service : Mode::Closed;
}

void processClosedMode();
void processAutoMode();
void processManualMode();
void processServiceMode();

void loop() {
	button.process();
	encButton.process();
	encoder.process();
	caseOpenDetector.process();

	if (!caseOpenDetector.isDown())
		appMode = Mode::Closed;

	if (appMode == Mode::Closed)
		processClosedMode();
	else if (appMode == Mode::Service)
		processServiceMode();
	else if (appMode == Mode::Auto)
		processAutoMode();
	else if (appMode == Mode::Manual)
		processManualMode();
}

void processClosedMode()
{
	pump.off();
	servo.write(0);
	display.setSegments(display_line, 4, 0);
	pixels.fill(pixels.gamma32(Color::Interpolate(Color::RED, Color::BLACK, pixels.sine8(millis() / 6))));
	pixels.show();
}

void processAutoMode()
{

}

void processManualMode()
{

}

void processServiceMode()
{

}
