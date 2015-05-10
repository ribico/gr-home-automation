/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#include "NTC.h"

// These constants won't change.  They're used to give names
// to the pins used:


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
	float temp = NTC_RawADCToCelsius( analogRead(A0) );
	Serial.print("temp A0 = ");
	Serial.print(temp);

	temp = NTC_RawADCToCelsius( analogRead(A1) );
	Serial.print("\t temp A1 = ");
	Serial.print(temp);

	temp = NTC_RawADCToCelsius( analogRead(A2) );
	Serial.print("\t temp A2 = ");
	Serial.print(temp);

	temp = NTC_RawADCToCelsius( analogRead(A3) );
	Serial.print("\t temp A3 = ");
	Serial.print(temp);

	temp = NTC_RawADCToCelsius( analogRead(A4) );
	Serial.print("\t temp A4 = ");
	Serial.print(temp);

	temp = NTC_RawADCToCelsius( analogRead(A5) );
	Serial.print("\t temp A5 = ");
	Serial.println(temp);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(100);
}