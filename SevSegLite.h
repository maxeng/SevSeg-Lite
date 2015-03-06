/* SevSeg-Lite Library
 
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
 */

// If you use current-limiting resistors on your segment pins instead of the
// digit pins, then change the '0' in the line below to a '1'
#define RESISTORS_ON_SEGMENTS 0
#define MAXNUMDIGITS 8 //Increase this number to support larger displays


#ifndef SevSeg_h
#define SevSeg_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Use defines to link the hardware configurations to the correct numbers
#define COMMON_CATHODE 0
#define COMMON_ANODE 1


class SevSeg
{
public:
  SevSeg();

  void refreshDisplay();
  void begin(byte hardwareConfig, byte numDigitsIn, byte digitPinsIn[], byte segmentPinsIn[]);
  void setBrightness(int brightnessIn); // A number from 0..100

  void setNumber(long numToShow);
  void setNumber(unsigned long numToShow);
  void setNumber(int numToShow);
  void setNumber(unsigned int numToShow);
  void setNumber(char numToShow);
  void setNumber(byte numToShow);
  void setNumber(float numToShow);

private:
  void setNewNum(long numToShow);
  void findDigits(long numToShow, byte nums[]);
  void setDigitCodes(byte nums[]);

  boolean digitOn,digitOff,segmentOn,segmentOff;
  byte digitPins[MAXNUMDIGITS];
  byte segmentPins[7]; //no decimal
  byte numDigits;
  byte digitCodes[MAXNUMDIGITS];
  int ledOnTime;
  const static long powersOf10[10];

};

#endif //SevSeg_h
/// END ///
