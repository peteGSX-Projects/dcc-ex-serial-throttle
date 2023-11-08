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
  display.begin();
  display.setFont(DEFAULT_FONT);
  display.clear();
  display.setFont(DCCEX_FONT);
  display.drawStr(0, 15, "DCC-EX");
  display.setFont(DEFAULT_FONT);
  display.drawStr(0, 24, "Serial Throttle");
  display.drawStr(0, 33, VERSION);
  display.sendBuffer();
}

void displayKey(char key, KeyState keyState) {
  display.setFont(MENU_ITEM_FONT);
  display.setCursor(55, 63);
  display.print(F("  "));
  display.setCursor(55, 63);
  display.print(key);
  if (keyState==HOLD) {
    display.print(F("H"));
  } else if (keyState==RELEASED) {
    display.print(F("R"));
  } else {
    display.print(F("P"));
  }
  display.sendBuffer();
}

void displayConnectionError() {
  display.clear();
  display.setFont(DEFAULT_FONT);
  display.drawStr(30, 10, "Connection error");
  display.setFont(EMOJIS);
  display.drawGlyphX2(48, 50, EmojiSadFace);
  display.sendBuffer();
}

void displayThrottleSpeed(int throttle, int speed) {
  display.setFont(SPEED_FONT);
  display.setCursor(
    throttleSetup[throttle].speed.textColumn,
    throttleSetup[throttle].speed.startRow
  );
  display.print(speed);
  display.sendBuffer();
}

void displayThrottleDirection(int throttle, Direction direction) {
  display.setFont(DIRECTION_FONT);
  display.setCursor(
    throttleSetup[throttle].direction.textColumn,
    throttleSetup[throttle].direction.startRow
  );
  if (direction==Forward) {
    display.print(F("Fwd"));
  } else {
    display.print(F("Rev"));
  }
  display.sendBuffer();
}

void displayThrottleAddress(int throttle, int address, bool isOverridden, bool isConsist) {
  display.setFont(ADDRESS_FONT);
  display.setCursor(
    throttleSetup[throttle].address.textColumn,
    throttleSetup[throttle].address.startRow
  );
  if (isOverridden) {
    display.print(F("*"));
  } else {
    display.print(F(" "));
  }
  display.print(address);
  if (isConsist) {
    display.print(F("c"));
  }
  display.sendBuffer();
}

void displayThrottleEStop(int throttle) {
  display.setFont(ESTOP_FONT);
  display.setCursor(
    throttleSetup[throttle].speed.textColumn,
    throttleSetup[throttle].speed.startRow
  );
  display.print(F("ESTOP"));
  display.sendBuffer();
}
