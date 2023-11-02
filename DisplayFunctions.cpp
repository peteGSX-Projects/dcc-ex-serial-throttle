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
// #ifdef USE_OLED_SPI
// SSD1306AsciiSpi oled;
// #endif
// #ifdef USE_OLED_I2C
// SSD1306AsciiAvrI2c oled;
// #endif
OLED oled;

void displayStartupInfo() {
  // Set up serial and display basic config
  Serial.print(F("DCC-EX Serial Throttle "));
  Serial.println(VERSION);
  // Serial.print(F("Averaging inputs over "));
  // Serial.print(SAMPLES);
  // Serial.println(F(" samples"));
  // Set up OLED
#ifdef USE_OLED_SPI
  oled.begin(OLED_TYPE, CS_PIN, DC_PIN);
#endif
#ifdef USE_OLED_I2C
  oled.begin(OLED_TYPE, OLED_ADDRESS);
#endif
  oled.setFont(OLED_FONT);
  oled.clear();
  oled.setCursor(0, 0);
  oled.set1X();
  oled.print("DCC-EX");
  oled.setCursor(0, 2);
  oled.print("Serial Throttle");
  oled.setCursor(0, 4);
  oled.print(VERSION);
}

void displayKey(char key, KeyState keyState) {
  oled.clear(55, 69, 7, 7);
  oled.setCursor(55, 7);
  oled.set1X();
  oled.print(key);
  if (keyState==HOLD) {
    oled.print(F("H"));
  } else if (keyState==RELEASED) {
    oled.print(F("R"));
  } else {
    oled.print(F("P"));
  }
}

/*
void displayThrottleSpeed(int startCol, int endCol, int textCol, int speed) {
  if (currentMenuPtr->getParent() != nullptr) return;
  oled.set2X();
  oled.clear(startCol, endCol, 0, 2);
  oled.setCursor(textCol, 0);
  oled.print(speed);
}

void displayThrottleDirection(int startCol, int endCol, int textCol, bool direction) {
  if (currentMenuPtr->getParent() != nullptr) return;
  oled.set1X();
  oled.clear(startCol, endCol, 3, 4);
  oled.setCursor(textCol, 3);
  if (direction == Forward) {
    oled.print(F("Fwd"));
  } else {
    oled.print(F("Rev"));
  }
}

void displayThrottleAddress(int startCol, int endCol, int textCol, int address, bool isOverridden, bool isConsist) {
  if (currentMenuPtr->getParent() != nullptr) return;
  oled.set1X();
  oled.clear(startCol, endCol, 5, 5);
  oled.setCursor(textCol, 5);
  if (isOverridden) {
    oled.print("*");
  } else {
    oled.print(" ");
  }
  oled.print(address);
  if (isConsist) oled.print("c");
}

void displayThrottle1Speed() {
  displayThrottleSpeed(0, 45, 4, throttle1.getSpeed());
}

void displayThrottle2Speed() {
  displayThrottleSpeed(46, 87, 46, throttle2.getSpeed());
}

void displayThrottle3Speed() {
  displayThrottleSpeed(88, 128, 88, throttle3.getSpeed());
}

void displayThrottle1Direction() {
  displayThrottleDirection(0, 51, 10, throttle1.getDirection());
}

void displayThrottle2Direction() {
  displayThrottleDirection(52, 93, 52, throttle2.getDirection());
}

void displayThrottle3Direction() {
  displayThrottleDirection(94, 128, 94, throttle3.getDirection());
}

void displayThrottle1Address() {
  displayThrottleAddress(0, 42, 0, throttle1.getLocoAddress(), throttle1.isOverridden(), throttle1.isConsist());
}

void displayThrottle2Address() {
  displayThrottleAddress(43, 86, 43, throttle2.getLocoAddress(), throttle2.isOverridden(), throttle2.isConsist());
}

void displayThrottle3Address() {
  displayThrottleAddress(87, 128, 87, throttle3.getLocoAddress(), throttle3.isOverridden(), throttle3.isConsist());
}

void displayHomeScreen() {
  oled.clear();
  displayThrottle1Speed();
  displayThrottle2Speed();
  displayThrottle3Speed();
  displayThrottle1Direction();
  displayThrottle2Direction();
  displayThrottle3Direction();
  displayThrottle1Address();
  displayThrottle2Address();
  displayThrottle3Address();
  oled.setCursor(0, 7);
  oled.print("* Menu");
} 

void displayConnectionError() {
  oled.clear();
  oled.setCursor(0, 0);
  oled.set1X();
  oled.print("Connection error");
}
*/

/*
Display EStop status
*/
// void displayEStop() {
//   oled.setCursor(0, 0);
//   oled.clearToEOL();
//   oled.setCursor(4, 1);
//   oled.set1X();
//   oled.print("ESTOP");
//   oled.clearToEOL();
//   oled.setCursor(46, 1);
//   oled.print("ESTOP");
//   oled.clearToEOL();
//   oled.setCursor(88, 1);
//   oled.print("ESTOP");
//   oled.clearToEOL();
// }
