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

#include "DisplayFunctions.h"
#include "KeypadFunctions.h"
#include "SerialFunctions.h"
// #include "AnalogueAverage.h"
#include "DCCEXObjects.h"
#include "Menu.h"
#include "Throttle.h"

// AnalogueAverage pot1(POT1_PIN);
// AnalogueAverage pot2(POT2_PIN);
// AnalogueAverage pot3(POT3_PIN);


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
  createMenus();
  currentMenuPtr->display();
}

/***********************************************************************************
Main loop
***********************************************************************************/
void loop() {
  dccexProtocol.check();
  validateConnection();
  updateRoster();
  updateRoutes();
  updateTurnouts();
  updateTurntables();
  // pot1.averageInput();
  // pot2.averageInput();
  // pot3.averageInput();
  // newLoco1Speed = map(pot1.getAverage(), POT_MIN, POT_MAX, 0, 126);
  // newLoco2Speed = map(pot2.getAverage(), POT_MIN, POT_MAX, 0, 126);
  // newLoco3Speed = map(pot3.getAverage(), POT_MIN, POT_MAX, 0, 126);
  // bool updateSpeeds = false;
  // if (newLoco1Speed != loco1Speed) {
  //   loco1Speed = newLoco1Speed;
  //   updateSpeeds = true;
  // }
  // if (newLoco2Speed != loco2Speed) {
  //   loco2Speed = newLoco2Speed;
  //   updateSpeeds = true;
  // }
  // if (newLoco3Speed != loco3Speed) {
  //   loco3Speed = newLoco3Speed;
  //   updateSpeeds = true;
  // }
  // if (updateSpeeds) {
  //   currentMenuPtr->display();
  // }
  char key = keypad.getKey();
  if (key) {
    currentMenuPtr->handleKeyPress(key);
  }
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
