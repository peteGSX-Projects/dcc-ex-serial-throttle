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

#include <Arduino.h>
#include "defines.h"
#include "DisplayFunctions.h"
#include "DCCEXObjects.h"

DCCEXProtocol dccexProtocol;
DCCEXCallbacks dccexCallbacks;

bool connectionRequested = false;
bool connected = false;
bool errorDisplayed = false;
uint8_t connectionRetries = 5;
unsigned long retryDelay = 500;
unsigned long lastRetry = 0;
bool objectsRequested = false;
bool requestedRoster = false;
bool gotRoster = false;
bool requestedRoutes = false;
bool gotRoutes = false;
bool requestedTurnouts = false;
bool gotTurnouts = false;
bool requestedTurntables = false;
bool gotTurntables = false;

/*
Function to validate a connection to the CS has been made
*/
void validateConnection() {
  if (!connected) {
    if (!connectionRequested) {
      connectionRequested = true;
      dccexProtocol.sendServerDetailsRequest();
      CONSOLE.println(F("Connecting to DCC-EX"));
    } else if (dccexProtocol.isServerDetailsReceived()) {
      CONSOLE.print(F("Connected to DCC-EX"));
      connected = true;
    } else if (millis() - lastRetry > retryDelay && connectionRetries > 0) {
      lastRetry = millis();
      connectionRetries--;
    } else if (connectionRetries == 0 && !errorDisplayed) {
      errorDisplayed = true;
      displayConnectionError();
    }
  }
}

/*
Function to update roster entries from the CS
To trigger after startup, simply set requestedRoster to false
*/
void updateRoster() {
  if (!requestedRoster) {
    requestedRoster = true;
    dccexProtocol.getRoster();
  } else if (dccexProtocol.isRosterFullyReceived()  && !gotRoster && requestedRoster) {
    gotRoster = true;
    for (int i = 0; i < dccexProtocol.roster.size(); i++) {
      Loco* loco = dccexProtocol.roster.get(i);
      const char* locoName = loco->getLocoName().c_str();
      int locoAddress = loco->getLocoAddress();
      rosterList.addItem(i, locoName, locoAddress, dummy);
      delete loco;
    }
  }
}

/*
Function to update route entries from the CS
To trigger after startup, simply set requestedRoutes to false
*/
void updateRoutes() {
  if (!requestedRoutes && gotRoster) {
    requestedRoutes = true;
    dccexProtocol.getRoutes();
  } else if (dccexProtocol.isRouteListFullyReceived()  && !gotRoutes && requestedRoutes) {
    gotRoutes = true;
    for (int i = 0; i < dccexProtocol.routes.size(); i++) {
      Route* route = dccexProtocol.routes.get(i);
      const char* routeName = route->getRouteName().c_str();
      int routeId = route->getRouteId();
      routeList.addItem(i, routeName, routeId, dummy);
      delete route;
    }
  }
}

/*
Function to update turnout entries from the CS
To trigger after startup, simply set requestedTurnouts to false
*/
void updateTurnouts() {
  if (!requestedTurnouts && gotRoster && gotRoutes) {
    requestedTurnouts = true;
    dccexProtocol.getTurnouts();
  } else if (dccexProtocol.isTurnoutListFullyReceived()  && !gotTurnouts && requestedTurnouts) {
    gotTurnouts = true;
    for (int i = 0; i < dccexProtocol.turnouts.size(); i++) {
      Turnout* turnout = dccexProtocol.turnouts.get(i);
      const char* turnoutName = turnout->getTurnoutName().c_str();
      int turnoutId = turnout->getTurnoutId();
      turnoutList.addItem(i, turnoutName, turnoutId, operateTurnout);
      delete turnout;
    }
  }
}

/*
Function to update turntable entries from the CS
To trigger after startup, simply set requestedTurntables to false
*/
void updateTurntables() {
  if (!requestedTurntables && gotRoster && gotRoutes && gotTurnouts) {
    requestedTurntables = true;
    dccexProtocol.getTurntables();
  } else if (dccexProtocol.isTurntableListFullyReceived()  && !gotTurntables && requestedTurntables) {
    gotTurntables = true;
    for (int i = 0; i < dccexProtocol.turntables.size(); i++) {
      Turntable* turntable = dccexProtocol.turntables.get(i);
      const char* turntableName = turntable->getTurntableName().c_str();
      int turntableId = turntable->getTurntableId();
      turntableList.addItem(i, turntableName, turntableId, dummy);
      delete turntable;
    }
  }
}

/*
Function to operate a turnout
If closed, will throw, if thrown, will close
*/
void operateTurnout() {
  if (currentMenuPtr != nullptr) {
    int16_t objectId = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
    CONSOLE.print(F("Toggle turnout ID "));
    CONSOLE.println(objectId);
    for (int i = 0; i < dccexProtocol.turnouts.size(); i++) {
      Turnout* turnout = dccexProtocol.turnouts.get(i);
      int turnoutId = turnout->getTurnoutId();
      if (turnoutId == objectId) {
        CONSOLE.println(turnout->getTurnoutState());
        turnout->setTurnoutState(TurnoutToggle);
      }
      // delete turnout;
    }
  }
}
