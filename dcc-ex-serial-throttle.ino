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
#include "StaticMenus.h"
#include "SerialFunctions.h"
#include "DCCEXObjects.h"
#include "Throttle.h"
#include "ThrottleSetup.h"
#include "MenuSystem.h"

// Array to hold all throttle objects to process
Throttle* throttles[NUM_THROTTLES];

unsigned long delayUntil=0;
unsigned long retrievalDelay=1000;
bool doOnce=true;

/***********************************************************************************
Main setup function
***********************************************************************************/
void setup() {
#if defined(ARDUINO_BLUEPILL_F103C8)
  disableJTAG();
#endif
  // Set up serial and display basic config
  CONSOLE.begin(115200);
#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
  CLIENT.begin(115200);
#endif
  displayStartupInfo();
  delay(2000);
  setupKeypad();
  setupThrottles();
  createMenus();
  menuSystem.setThrottles(throttles);
  dccexProtocol.setLogStream(&CONSOLE);
  dccexProtocol.setDelegate(&dccexCallbacks);
  dccexProtocol.connect(&CLIENT);
  delayUntil=millis()+retrievalDelay;
}

/***********************************************************************************
Main loop
***********************************************************************************/
void loop() {
  dccexProtocol.check();
  // validateConnection();
  // if (connected) {
    // if (doOnce) {
    //   doOnce=false;
    //   dccexProtocol.sendTrackPower(PowerOn);
    // }
    dccexProtocol.getLists(true, true, true, true);
    updateRoster();
    updateRoutes();
    updateTurnouts();
    updateTurntables();
  // }
  for (int i=0; i<NUM_THROTTLES; i++) {
    throttles[i]->process();
    if (throttles[i]->speedChanged()) {
      throttles[i]->displaySpeed(menuSystem.isHome());
    }
  }
  keypad.getKey();
  getSerialInput();
}

// Disabling JTAG is required to avoid pin conflicts on Bluepill
#if defined(ARDUINO_BLUEPILL_F103C8)
void disableJTAG() {
  // Disable JTAG and enable SWD by clearing the SWJ_CFG bits
  // Assuming the register is named AFIO_MAPR or AFIO_MAPR2
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG);
  // or
  // AFIO->MAPR2 &= ~(AFIO_MAPR2_SWJ_CFG);
}
#endif

// Setup the correct number of throttles as defined
void setupThrottles() {
  for (int i=0; i<NUM_THROTTLES; i++) {
    throttles[i]=new Throttle(i, throttleSetup[i].potPin, nullptr, display);
  }
}
