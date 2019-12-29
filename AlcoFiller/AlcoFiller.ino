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

#define SERVO_FB_ALLOWED_DELTA 10
#define IR_DETECT_THRESHOLD_VALUE (200)
#define PROP_MS_TO_ML 170L

#pragma region Peripherals

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

#pragma endregion
#pragma region Display characters

const byte display_line[] = { 0x40, 0x40, 0x40, 0x40 };
const byte display_Err[] = { 0x79, 0x50, 0x50, 0x00 };
const byte display_POS[] = { 0x73, 0x3F, 0x6D };
const byte display_A[] = { 0x77 };
const byte display_P[] = { 0x73 };

#pragma endregion
#pragma region Function declarations

void onStartClosedMode();
void onLoopClosedMode();
void onExitClosedMode();

void onStartServiceMode();
void onLoopServiceMode();
void onExitServiceMode();

void onStartAutoMode();
void onLoopAutoMode();
void onExitAutoMode();

void onStartManualMode();
void onLoopManualMode();
void onExitManualMode();

void loadConfig();
void saveConfig();

#pragma endregion
#pragma region Modes

namespace Mode
{
	typedef struct
	{
		void(*onStart)(void);
		void(*onLoop)(void);
		void(*onExit)(void);
	} Mode;

	namespace ModesPriv
	{
		Mode Closed = { onStartClosedMode, onLoopClosedMode, onExitClosedMode };
		Mode Service = { onStartServiceMode, onLoopServiceMode, onExitServiceMode };
		Mode Auto = { onStartAutoMode, onLoopAutoMode, onExitAutoMode };
		Mode Manual = { onStartManualMode, onLoopManualMode, onExitManualMode };
	}

	Mode* Closed = &ModesPriv::Closed;
	Mode* Service = &ModesPriv::Service;
	Mode* Auto = &ModesPriv::Auto;
	Mode* Manual = &ModesPriv::Manual;
}

#pragma endregion
#pragma region Shot states

namespace ShotState
{
	typedef struct
	{
		boolean isPresent;
		boolean isAdded;
		boolean isRemoved;
		byte volume;
	} ShotState;
}

#pragma endregion
#pragma region Global variables

Mode::Mode* appMode;
Mode::Mode* appModeBeforeClose;
int servoPos[] = { 150, 111, 69, 30 };
int servoFbs[] = { 0, 0, 0, 0 };
int currentDrinkVolume = 50;
ShotState::ShotState shots[4];

#pragma endregion
#pragma region Main loop and init

void rotateServoAndWait(int angle)
{
	int cur = servo.read();
	servo.write(angle);
	delay(abs(angle - cur) * 2.5);
}

void changeModeTo(Mode::Mode* mode)
{
	if (appMode != NULL)
		appMode->onExit();
	appMode = mode;
	appMode->onStart();
}

void processShots()
{
	int irVals[4] = {};

	ledIR.off();
	for (int i = 0; i < 4; i++)
		irVals[i] = rcvIR[i].getValue();

	ledIR.on();
	for (int i = 0; i < 4; i++)
		irVals[i] -= rcvIR[i].getValue();

	for (int i = 0; i < 4; i++)
	{
		boolean oldVal = shots[i].isPresent;
		boolean newVal = abs(irVals[i]) > IR_DETECT_THRESHOLD_VALUE;
		shots[i].isPresent = newVal;
		shots[i].isAdded = newVal && !oldVal;
		shots[i].isRemoved = oldVal && !newVal;

		if (oldVal != newVal)
			shots[i].volume = 0;
	}
}

void setup() {
	Serial.begin(115200);
	pixels.begin();
	servo.attach(Pins::SERVO_CONTROL);
	display.setBrightness(7, true);
	button.process();
	loadConfig();
	changeModeTo(Mode::Manual);
	if (button.isDown())
		changeModeTo(Mode::Service);
}

void loop() {
	button.process();
	encButton.process();
	encoder.process();
	caseOpenDetector.process();
	processShots();

	if (!caseOpenDetector.isDown() && appMode != Mode::Closed)
	{
		appModeBeforeClose = appMode;
		changeModeTo(Mode::Closed);
	}
	else if (caseOpenDetector.isDown() && appMode == Mode::Closed)
	{
		changeModeTo(appModeBeforeClose);
	}

	if (appMode != NULL)
		appMode->onLoop();
}

#pragma endregion
#pragma region Config

void loadConfig()
{
	int idx = 0;
	for (int i = 0; i < 4; i++)
	{
		EEPROM.get(idx, servoPos[i]);
		idx += sizeof(servoPos[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		EEPROM.get(idx, servoFbs[i]);
		idx += sizeof(servoFbs[i]);
	}
}

void saveConfig()
{
	int idx = 0;
	for (int i = 0; i < 4; i++)
	{
		EEPROM.put(idx, servoPos[i]);
		idx += sizeof(servoPos[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		EEPROM.put(idx, servoFbs[i]);
		idx += sizeof(servoFbs[i]);
	}
}

#pragma endregion
#pragma region Closed mode

void onStartClosedMode()
{
	pump.off();
	servo.write(0);
	display.setSegments(display_line, 4, 0);
}

void onLoopClosedMode()
{
	pixels.fill(pixels.gamma32(Color::Interpolate(Color::RED, Color::BLACK, pixels.sine8(millis() / 4))));
	pixels.show();
}

void onExitClosedMode() { }

#pragma endregion
#pragma region Auto mode

void onStartAutoMode()
{
	pump.off();
	servo.write(0);
	display.setSegments(display_A, 1, 0);
	display.showNumberDec(currentDrinkVolume, false, 3, 1);
}

void onLoopAutoMode()
{
	if (encButton.isPushed())
		changeModeTo(Mode::Manual);

	if (encoder.control(currentDrinkVolume, 0, 500, 5))
		display.showNumberDec(currentDrinkVolume, false, 3, 1);
}

void onExitAutoMode()
{

}

#pragma endregion
#pragma region Manual mode

void onStartManualMode()
{
	pump.off();
	servo.write(0);
	display.setSegments(display_P, 1, 0);
	display.showNumberDec(currentDrinkVolume, false, 3, 1);
	for (int i = 0; i < 4; i++)
		shots[i].volume = 0;
}

void onLoopManualMode()
{
	if (encButton.isPushed())
		changeModeTo(Mode::Auto);

	if(encoder.control(currentDrinkVolume, 0, 500, 5))
		display.showNumberDec(currentDrinkVolume, false, 3, 1);

	for (int i = 0; i < 4; i++)
	{
		if (shots[i].isPresent)
			pixels.setPixelColor(i, Color::Interpolate(Color::GREEN, Color::RED, shots[i].volume));
		else
			pixels.setPixelColor(i, Color::BLUE);
	}
	pixels.show();

	if (button.isPushed())
	{
		int vol = currentDrinkVolume;
		for (int i = 0; i < 4; i++)
		{
			ShotState::ShotState* sh = &shots[i];
			if (sh->isPresent && sh->volume == 0)
			{
				sh->volume = 0;
				rotateServoAndWait(servoPos[i]);

				long startTime = millis();
				long endTime = startTime + vol * PROP_MS_TO_ML;
				pump.on();
				while (millis() <= endTime)
				{
					processShots();
					button.process();
					caseOpenDetector.process();
					if (sh->isRemoved || button.isPushed())
						break;

					if (!caseOpenDetector.isDown())
					{
						i = 10;
						break;
					}

					unsigned long currTime = millis();
					int vol = map(currTime, startTime, endTime, 0, 255);
					sh->volume = min(vol, 255);

					for (int j = 0; j < 4; j++)
					{
						if (shots[j].isPresent)
							pixels.setPixelColor(j, Color::Interpolate(Color::GREEN, Color::RED, shots[j].volume));
						else
							pixels.setPixelColor(j, Color::BLACK);
					}						
					pixels.show();
				}
				pump.off();
				delay(100);
			}
		}
		servo.write(0);
	}
}

void onExitManualMode() { }

#pragma endregion
#pragma region Service Mode

namespace SvcMode
{
	typedef enum
	{
		None = -1,
		Slot1 = 0,
		Slot2 = 1,
		Slot3 = 2,
		Slot4 = 3
	} SvcMode;
}

SvcMode::SvcMode svcMode = SvcMode::None;

void onStartServiceMode()
{
	pump.off();
}

void onLoopServiceMode()
{
	if (button.isPushed() || svcMode == SvcMode::None)
	{
		svcMode = svcMode == SvcMode::Slot4 ? SvcMode::Slot1 : (SvcMode::SvcMode)(svcMode + 1);
		if (svcMode >= SvcMode::Slot1 && svcMode <= SvcMode::Slot4)
		{
			servoFbs[svcMode] = servoFb.getValue();
			servo.write(servoPos[svcMode]);
			display.setSegments(display_POS, 3, 0);
			display.showNumberDec(svcMode + 1, false, 1, 3);
			pixels.clear();
			pixels.setPixelColor(svcMode, Color::WHITE);
			pixels.show();
		}
	}

	if (encButton.isHold())
	{
		saveConfig();
		changeModeTo(Mode::Manual);
	}

	switch (svcMode)
	{
		case SvcMode::Slot1:
		case SvcMode::Slot2:
		case SvcMode::Slot3:
		case SvcMode::Slot4:
			if (encoder.control(servoPos[svcMode], 0, 180, 1))
			{
				servo.write(servoPos[svcMode]);
			}
			break;
	}
}

void onExitServiceMode() { }

#pragma endregion
