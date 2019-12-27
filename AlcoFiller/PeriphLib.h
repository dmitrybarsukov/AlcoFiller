#ifndef PERIPHLIB_H
#define PERIPHLIB_H

#include <Arduino.h>

/* Initialize all simple peripherals */
void initAll();

/* Set pump on or off */
void setPump(byte state);

/* Control IR leds */
void setIRLeds(byte state);

/* Get value from IR photodiodes */
int readIRvalue(byte index);

byte readBigButton

#endif // PERIPHLIB_H
