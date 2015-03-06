/* SevSegLite Library
 
 modified 2015 maxeng
 Lite version is omitted following features:
	- decimal point support
	- internal current-limiting resistors support (use external resistors)
	- external transistor and mosfet support
 also number of digit support is only 4.
 
 COMMON-CATHODE is default type of display. No need set option. (ANODE type must be set option.)
 setNumber() make auto refresh. no need refresh().
 
 --- original document is below ---------------------------------------------
 
 Copyright 2014 Dean Reading
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 
 This library allows an Arduino to easily display numbers in decimal format on
 a 7-segment display without a separate 7-segment display controller.
 
 Direct any questions or suggestions to deanreading@hotmail.com
 See the included readme for instructions.
 
 CHANGELOG
 
 Version 3.0 - November 2014
 Library re-design. A display with any number of digits can be used.
 Floats are supported. Support for using transistors for switching.
 Much more user friendly. No backwards compatibility.
 Uploaded to GitHub to simplify any further development.
 Version 2.3; Allows for brightness control.
 Version 2.2; Allows 1, 2 or 3 digit displays to be used.
 Version 2.1; Includes a bug fix.
 Version 2.0; Now works for any digital pin arrangement.
 Supports both common anode and common cathode displays.
 */

#include "SevSegLite.h"

#define BLANK 10 // Must match with 'digitCodeMap', defined in 'setDigitCodes' 
#define DASH 11


const long SevSegLite::powersOf10[] = {
  1, // 10^0
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
  1000000000}; // 10^9


// SevSegLite
/******************************************************************************/
SevSegLite::SevSegLite()
{
  // Initial value
  ledOnTime = 2000; // Corresponds to a brightness of 100
  numDigits = 0;
}


// begin
/******************************************************************************/
// Saves the input pin numbers to the class and sets up the pins to be used.

void SevSegLite::begin(byte numDigitsIn, byte digitPinsIn[], 
				  byte segmentPinsIn[]) {
  numDigits = numDigitsIn;
  //Limit the max number of digits to prevent overflowing
  if (numDigits > MAXNUMDIGITS) numDigits = MAXNUMDIGITS;
  
  // Common cathode
  digitOn = LOW;
  segmentOn = HIGH;
  
  digitOff = !digitOn;
  segmentOff = !segmentOn;

  // Save the input pin numbers to library variables
  for (byte segmentNum = 0 ; segmentNum < 7 ; segmentNum++) {
    segmentPins[segmentNum] = segmentPinsIn[segmentNum];
  }

  for (byte digitNum = 0 ; digitNum < numDigits ; digitNum++) {
    digitPins[digitNum] = digitPinsIn[digitNum];
  }

  // Set the pins as outputs, and turn them off
  for (byte digit=0 ; digit < numDigits ; digit++) {
    pinMode(digitPins[digit], OUTPUT);
    digitalWrite(digitPins[digit], digitOff);
  }

  for (byte segmentNum=0 ; segmentNum < 7 ; segmentNum++) {
    pinMode(segmentPins[segmentNum], OUTPUT);
    digitalWrite(segmentPins[segmentNum], segmentOff);
  }

  setNewNum(0); // Initialise the number displayed to 0
 
}

void SevSegLite::begin(byte hardwareConfig, byte numDigitsIn, 
                  byte digitPinsIn[], byte segmentPinsIn[]) {
                    
  numDigits = numDigitsIn;
  //Limit the max number of digits to prevent overflowing
  if (numDigits > MAXNUMDIGITS) numDigits = MAXNUMDIGITS;

  switch (hardwareConfig){

  case 0: // Common cathode
    digitOn = LOW;
    segmentOn = HIGH;
    break;

  case 1: // Common anode
    digitOn = HIGH;
    segmentOn = LOW;
    break;

  }

  digitOff = !digitOn;
  segmentOff = !segmentOn;

  // Save the input pin numbers to library variables
  for (byte segmentNum = 0 ; segmentNum < 7 ; segmentNum++) {
    segmentPins[segmentNum] = segmentPinsIn[segmentNum];
  }

  for (byte digitNum = 0 ; digitNum < numDigits ; digitNum++) {
    digitPins[digitNum] = digitPinsIn[digitNum];
  }

  // Set the pins as outputs, and turn them off
  for (byte digit=0 ; digit < numDigits ; digit++) {
    pinMode(digitPins[digit], OUTPUT);
    digitalWrite(digitPins[digit], digitOff);
  }

  for (byte segmentNum=0 ; segmentNum < 7 ; segmentNum++) {
    pinMode(segmentPins[segmentNum], OUTPUT);
    digitalWrite(segmentPins[segmentNum], segmentOff);
  }

  setNewNum(0); // Initialise the number displayed to 0
}


// refreshDisplay
/******************************************************************************/
// Flashes the output on the seven segment display.
// This is achieved by cycling through all segments and digits, turning the
// required segments on as specified by the array 'digitCodes'.


void SevSegLite::refreshDisplay(){
  for (byte digitNum=0 ; digitNum < numDigits ; digitNum++){

    // Illuminate the required segments for this digit
    digitalWrite(digitPins[digitNum], digitOn);
    for (byte segmentNum=0 ; segmentNum < 7 ; segmentNum++) {
      if (digitCodes[digitNum] & (1 << segmentNum)) { // Check a single bit
        digitalWrite(segmentPins[segmentNum], segmentOn);
      }
    }

    //Wait with lights on (to increase brightness)
    delayMicroseconds(ledOnTime);

    //Turn all lights off
    for (byte segmentNum=0 ; segmentNum < 7 ; segmentNum++) {
      digitalWrite(segmentPins[segmentNum], segmentOff);
    }
    digitalWrite(digitPins[digitNum], digitOff);
  }
}


// setBrightness
/******************************************************************************/

void SevSegLite::setBrightness(int brightness){
  brightness = constrain(brightness, 0, 100);
  ledOnTime = map(brightness, 0, 100, 1, 2000);
}


// setNumber
/******************************************************************************/
// This function only receives the input and passes it to 'setNewNum'.
// It is overloaded for all number data types, so that floats can be handled
// correctly.

void SevSegLite::setNumber(long numToShow) //long
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(unsigned long numToShow) //unsigned long
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(int numToShow) //int
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(unsigned int numToShow) //unsigned int
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(char numToShow) //char
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(byte numToShow) //byte
{
  setNewNum(numToShow);
}

void SevSegLite::setNumber(float numToShow) //float
{
  // numToShow = numToShow * powersOf10[decPlaces];
  // Modify the number so that it is rounded to an integer correctly
  numToShow += (numToShow >= 0) ? 0.5f : -0.5f;
  setNewNum(numToShow);
}


// setNewNum
/******************************************************************************/
// Changes the number that will be displayed.

void SevSegLite::setNewNum(long numToShow){
  byte digits[numDigits];
  findDigits(numToShow, digits);
  setDigitCodes(digits);
  refreshDisplay();
}


// findDigits
/******************************************************************************/
// Decides what each digit will display.
// Enforces the upper and lower limits on the number to be displayed.

void SevSegLite::findDigits(long numToShow, byte digits[]) {
  static const long maxNum = powersOf10[numDigits] - 1; //if numDigits = 2, 100-1= 99
  static const long minNum = -(powersOf10[numDigits - 1] - 1); //if numDigits = 2, -(10-1)= -9

  // If the number is out of range, just display dashes
  if (numToShow > maxNum || numToShow < minNum) {
    for (byte digitNum = 0 ; digitNum < numDigits ; digitNum++){
      digits[digitNum] = DASH;
    }
  }
  else{
    byte digitNum = 0;

    // Convert all number to positive values
    if (numToShow < 0) {
      digits[0] = DASH;
      digitNum = 1; // Skip the first iteration
      numToShow = -numToShow;
    }

    // Find all digits for the base 10 representation, starting with the most
    // significant digit
    for ( ; digitNum < numDigits ; digitNum++){
      long factor = powersOf10[numDigits - 1 - digitNum];
      digits[digitNum] = numToShow / factor;
      numToShow -= digits[digitNum] * factor;
    }

    // Find unnnecessary leading zeros and set them to BLANK
    for (digitNum = 0 ; digitNum < (numDigits - 1) ; digitNum++){
      if (digits[digitNum] == 0) {
        digits[digitNum] = BLANK;
      }
      // Exit once the first non-zero number is encountered
      else if (digits[digitNum] <= 9) {
        break;
      }
    }

  }
}


// setDigitCodes
/******************************************************************************/
// Sets the 'digitCodes' that are required to display the input numbers

void SevSegLite::setDigitCodes(byte digits[]) {

  // The codes below indicate which segments must be illuminated to display
  // each number.
  static const byte digitCodeMap[] = {
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111, // 9
    B00000000, // BLANK
    B01000000    }; // DASH

  // Set the digitCode for each digit in the display
  for (byte digitNum = 0 ; digitNum < numDigits ; digitNum++) {
    digitCodes[digitNum] = digitCodeMap[digits[digitNum]];
    // Set the decimal place segment
    // if (digitNum == numDigits - 1 - decPlaces) { //if digitNum = 2, 2-1 = 1
     // digitCodes[digitNum] |= B10000000;
    // }
  }
}

/// END ///
