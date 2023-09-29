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
#include "DCCEXCallbacks.h"
#include "NA_SerialFunctions.h"
#include "AnalogueAverage.h"

AnalogueAverage pot1(POT1_PIN);

DCCEXProtocol dccexProtocol;
DCCEXCallbacks dccexCallbacks;

bool gotRoster = false;
bool gotTurnouts = false;
bool gotTurntables = false;
bool gotRoutes = false;

/***********************************************************************************
Main setup function
***********************************************************************************/
void setup() {
#if defined(ARDUINO_BLUEPILL_F103C8)
  disableJTAG();
#endif
  pinMode(POT1_PIN, INPUT);
  // Set up serial and display basic config
  CONSOLE.begin(115200);
#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
  CLIENT.begin(115200);
#endif
  displayStartupInfo();
  delay(2000);
  setupKeypad();
  dccexProtocol.setLogStream(&CONSOLE);
  dccexProtocol.setDelegate(&dccexCallbacks);
  dccexProtocol.connect(&CLIENT);
  CONSOLE.print(F("Connected to DCC-EX"));
  dccexProtocol.sendServerDetailsRequest();
  dccexProtocol.getRoster();
  dccexProtocol.getTurnouts();
  dccexProtocol.getTurntables();
  dccexProtocol.getRoutes();
  displaySpeeds();
}

/***********************************************************************************
Main loop
***********************************************************************************/
void loop() {
  dccexProtocol.check();
  if (!dccexProtocol.isRosterFullyReceived()  && !gotRoster) {
    gotRoster = true;
    CONSOLE.println("Got the roster");
    oled.setCursor(0, 4);
    oled.print("Roster");
  }
  if (!dccexProtocol.isTurnoutListFullyReceived()  && !gotTurnouts) {
    gotTurnouts = true;
    CONSOLE.println("Got the turnouts");
    oled.setCursor(0, 5);
    oled.print("Turnouts");
  }
  if (!dccexProtocol.isTurntableListFullyReceived()  && !gotTurntables) {
    gotTurntables = true;
    CONSOLE.println("Got the turntables");
    oled.setCursor(0, 6);
    oled.print("Turntables");
  }
  if (!dccexProtocol.isRouteListFullyReceived()  && !gotRoutes) {
    gotRoutes = true;
    CONSOLE.println("Got the routes");
    oled.setCursor(0, 7);
    oled.print("Routes");
  }
  pot1.averageInput();
  if (pot1.getAverage() != loco1Speed) {
    loco1Speed = pot1.getAverage();
  }
  keypad.getKey();
  getSerialInput();
  displaySpeeds();
}

/*
Disabling JTAG is required to avoid pin conflicts on Bluepill
*/
#if defined(ARDUINO_BLUEPILL_F103C8)
void disableJTAG() {
  // Disable JTAG and enable SWD by clearing the SWJ_CFG bits
  // Assuming the register is named AFIO_MAPR or AFIO_MAPR2
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG);
  // or
  // AFIO->MAPR2 &= ~(AFIO_MAPR2_SWJ_CFG);
}
#endif
