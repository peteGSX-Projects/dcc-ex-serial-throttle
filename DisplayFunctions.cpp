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
  display.drawStr(0, 15, "DCC-EX");
  display.setFont(DEFAULT_FONT);
  display.drawStr(0, 24, "Serial Throttle");
  display.drawStr(0, 33, VERSION);
  display.sendBuffer();
}

void displayKey(char key, KeyState keyState) {
  display.setFont(MENU_ITEM_FONT);
  display.setCursor(55, 63);
  display.print("  ");
  display.setCursor(55, 63);
  display.print(key);
  if (keyState==HOLD) {
    display.print("H");
  } else if (keyState==RELEASED) {
    display.print("R");
  } else {
    display.print("P");
  }
  display.sendBuffer();
}

void displayConnectionError() {
  display.clear();
  display.setFont(DEFAULT_FONT);
  display.drawStr(30, 10, "Connection error");
  display.sendBuffer();
}

void displayThrottleSpeed(int throttle, int speed, bool isHome) {
  if (isHome) {
    display.setFont(SPEED_FONT);
    display.setCursor(
      throttleSetup[throttle].speed.X,
      throttleSetup[throttle].speed.Y
    );
    display.print("   ");
    display.setCursor(
      throttleSetup[throttle].speed.X,
      throttleSetup[throttle].speed.Y
    );
    display.print(speed);
    display.sendBuffer();
  }
}

void displayThrottleDirection(int throttle, Direction direction) {
  display.setFont(DIRECTION_FONT);
  display.setCursor(
    throttleSetup[throttle].direction.X,
    throttleSetup[throttle].direction.Y
  );
  display.print("   ");
  display.setCursor(
    throttleSetup[throttle].direction.X,
    throttleSetup[throttle].direction.Y
  );
  if (direction==Forward) {
    display.print("Fwd");
  } else {
    display.print("Rev");
  }
  display.sendBuffer();
}

void displayThrottleAddress(int throttle, int address, bool isOverridden, bool isConsist) {
  display.setFont(ADDRESS_FONT);
  display.setCursor(
    throttleSetup[throttle].address.X,
    throttleSetup[throttle].address.Y
  );
  display.print("       ");
  display.setCursor(
    throttleSetup[throttle].address.X,
    throttleSetup[throttle].address.Y
  );
  if (isOverridden) {
    display.print("*");
  } else {
    display.print(" ");
  }
  display.print(address);
  if (isConsist) {
    display.print("c");
  }
  display.sendBuffer();
}

void displayThrottleEStop(int throttle) {
  display.setFont(ESTOP_FONT);
  display.setCursor(
    throttleSetup[throttle].speed.X,
    throttleSetup[throttle].speed.Y
  );
  display.print("ESTOP");
  display.sendBuffer();
}
