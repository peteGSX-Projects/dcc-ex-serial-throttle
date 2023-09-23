/*
 *  © 2023 Peter Cole
 *  © 2022 Peter Cole
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

/***********************************************************************************
Include the required libraries
***********************************************************************************/
#include <Arduino.h>
#include "defines.h"

#include <DCCEXProtocol.h>
#include "DisplayFunctions.h"
#include "KeypadFunctions.h"
#include "SerialFunctions.h"

/***********************************************************************************
Main setup function
***********************************************************************************/
void setup() {
  // Set up serial and display basic config
  Serial.begin(115200);
  displayStartupInfo();
  setupKeypad();
}

/***********************************************************************************
Main loop
***********************************************************************************/
void loop() {
  getSerialInput();
  keypad.getKey();
}
