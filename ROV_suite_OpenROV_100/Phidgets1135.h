/*
 *Phidgets1135.h - Library for using the Phidgets 1135 Precision Voltage sensor.
 *Written by Chris Konstad 1/22/2011
 *Relased into the public domain
 *Helped by Jay Isaacs and Rolf Konstad
 *Accurate to within about 0.5 volts
 */


#ifndef Phidgets1135_h
#define Phidgets1135_h

#include "Arduino.h"

class Phidgets1135
{
  public:
    Phidgets1135(int voltagePin);
    float getValue();
	float getAverage(unsigned int phidgets1135Repeats);
  private:
    int _voltagePin;
    float _voltageInput;
    float _voltageTrue;
    float _voltageConvert;
};

#endif
