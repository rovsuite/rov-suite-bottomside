/*
 *Phidgets1135.cpp - Library for using the Phidgets 1135 Precision Voltage sensor.
 *Written by Chris Konstad 1/22/2011
 *Relased into the public domain
 */

#include "Arduino.h"
#include "Phidgets1135.h"

Phidgets1135::Phidgets1135(int voltagePin)
{
  pinMode(voltagePin, INPUT);
  _voltagePin = voltagePin;
}

float Phidgets1135::getValue()
{
  _voltageInput = analogRead(_voltagePin);
  _voltageConvert = map(_voltageInput,0,1023,0,999);
  _voltageTrue = (_voltageConvert / 13.62) - 36.7107 + 0.22;
}

float Phidgets1135::getAverage(unsigned int phidgets1135Repeats)
{
	float phidgets1135Answer = 0.0F;
	for(int i = 0; i < phidgets1135Repeats; i++)
	{
		_voltageInput = analogRead(_voltagePin);
		_voltageConvert = map(_voltageInput,0,1023,0,999);
		_voltageTrue = (_voltageConvert / 13.62) - 36.7107 + 0.22;
		phidgets1135Answer += _voltageTrue;
	}	
	phidgets1135Answer = (phidgets1135Answer / phidgets1135Repeats);
	return phidgets1135Answer;
}		