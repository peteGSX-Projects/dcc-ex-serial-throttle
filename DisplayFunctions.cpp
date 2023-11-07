/*
 *  © 2023 Peter Cole
 *
 *  This file is for a serially connected throttle for a DCC-EX EX-CommandStation.
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this code.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "DisplayFunctions.h"

/***********************************************************************************
Set up OLED libraries and object
***********************************************************************************/
OLED display(U8G2_R0, CS_PIN, DC_PIN);

void displayStartupInfo() {
  // Set up serial and display basic config
  CONSOLE.print(F("DCC-EX Serial Throttle "));
  CONSOLE.println(VERSION);
  // Set up OLED
// #ifdef USE_OLED_SPI
//   display.begin(OLED_TYPE, CS_PIN, DC_PIN);
// #endif
// #ifdef USE_OLED_I2C
//   display.begin(OLED_TYPE, OLED_ADDRESS);
// #endif
  display.begin();
  display.setFont(OLED_FONT);
  display.clear();
  display.setCursor(0, 0);
  // display.set1X();
  display.print("DCC-EX");
  display.setCursor(0, 2);
  display.print("Serial Throttle");
  display.setCursor(0, 4);
  display.print(VERSION);
  display.sendBuffer();
}

void displayKey(char key, KeyState keyState) {
  // display.clear(55, 69, 7, 7);
  display.setCursor(55, 7);
  // display.set1X();
  display.print(key);
  if (keyState==HOLD) {
    display.print(F("H"));
  } else if (keyState==RELEASED) {
    display.print(F("R"));
  } else {
    display.print(F("P"));
  }
}

void displayConnectionError() {
  display.clear();
  display.setCursor(0, 0);
  // display.set1X();
  display.print("Connection error");
}
