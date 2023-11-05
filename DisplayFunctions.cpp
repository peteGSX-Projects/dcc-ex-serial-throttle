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
OLED display;

void displayStartupInfo() {
  // Set up serial and display basic config
  CONSOLE.print(F("DCC-EX Serial Throttle "));
  CONSOLE.println(VERSION);
  // Set up OLED
#ifdef USE_OLED_SPI
  display.begin(OLED_TYPE, CS_PIN, DC_PIN);
#endif
#ifdef USE_OLED_I2C
  display.begin(OLED_TYPE, OLED_ADDRESS);
#endif
  display.setFont(OLED_FONT);
  display.clear();
  display.setCursor(0, 0);
  display.set1X();
  display.print("DCC-EX");
  display.setCursor(0, 2);
  display.print("Serial Throttle");
  display.setCursor(0, 4);
  display.print(VERSION);
}

void displayKey(char key, KeyState keyState) {
  display.clear(55, 69, 7, 7);
  display.setCursor(55, 7);
  display.set1X();
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
  display.set1X();
  display.print("Connection error");
}

/*
void displayThrottleSpeed(int startCol, int endCol, int textCol, int speed) {
  if (currentMenuPtr->getParent() != nullptr) return;
  display.set2X();
  display.clear(startCol, endCol, 0, 2);
  display.setCursor(textCol, 0);
  display.print(speed);
}

void displayThrottleDirection(int startCol, int endCol, int textCol, bool direction) {
  if (currentMenuPtr->getParent() != nullptr) return;
  display.set1X();
  display.clear(startCol, endCol, 3, 4);
  display.setCursor(textCol, 3);
  if (direction == Forward) {
    display.print(F("Fwd"));
  } else {
    display.print(F("Rev"));
  }
}

void displayThrottleAddress(int startCol, int endCol, int textCol, int address, bool isOverridden, bool isConsist) {
  if (currentMenuPtr->getParent() != nullptr) return;
  display.set1X();
  display.clear(startCol, endCol, 5, 5);
  display.setCursor(textCol, 5);
  if (isOverridden) {
    display.print("*");
  } else {
    display.print(" ");
  }
  display.print(address);
  if (isConsist) display.print("c");
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
  display.clear();
  displayThrottle1Speed();
  displayThrottle2Speed();
  displayThrottle3Speed();
  displayThrottle1Direction();
  displayThrottle2Direction();
  displayThrottle3Direction();
  displayThrottle1Address();
  displayThrottle2Address();
  displayThrottle3Address();
  display.setCursor(0, 7);
  display.print("* Menu");
} 


*/

/*
Display EStop status
*/
// void displayEStop() {
//   display.setCursor(0, 0);
//   display.clearToEOL();
//   display.setCursor(4, 1);
//   display.set1X();
//   display.print("ESTOP");
//   display.clearToEOL();
//   display.setCursor(46, 1);
//   display.print("ESTOP");
//   display.clearToEOL();
//   display.setCursor(88, 1);
//   display.print("ESTOP");
//   display.clearToEOL();
// }
