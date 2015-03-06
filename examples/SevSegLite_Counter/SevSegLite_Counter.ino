/* SevSegLite Counter Example
 
 modified 2015 maxeng
 Lite version is omitted following features:
	- decimal point support
	- internal current-limiting resistors support
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
 
 
 This example demonstrates a very simple use of the SevSegLite library with a 4
 digit display. It displays a counter that counts up, showing deci-seconds.
 */

#include "SevSegLite.h"

SevSegLite sevsegLite; //Instantiate a seven segment controller object

  
void setup() {
  byte numDigits = 2;
  byte digitPins[] = {8, 4};
  byte segmentPins[] = {1, 0, 23, 21, 20, 2, 22}; //dot point is not supported
  sevsegLite.begin(numDigits, digitPins, segmentPins); //COMMON-CATHODE
  sevsegLite.setBrightness(80);
}

void loop() {
  static int counter = -9;
	sevsegLite.setNumber(counter);
	
	counter ++;
	if (counter == 100) { // Reset to 0 after counting for 100.
		counter=-9;
	}
	delay(10);
}

/// END ///
