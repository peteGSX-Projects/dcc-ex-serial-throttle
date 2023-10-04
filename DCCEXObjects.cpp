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
    CONSOLE.println("Got roster");
    for (int i = 0; i < dccexProtocol.roster.size(); i++) {
      const char* locoName = dccexProtocol.roster.get(i)->getLocoName().c_str();
      int dccId = dccexProtocol.roster.get(i)->getLocoAddress();
      rosterList.addItem(i, locoName, dccId, dummy);
      CONSOLE.print("Roster index ");
      CONSOLE.print(i);
      CONSOLE.print(": ");
      CONSOLE.println(locoName);
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
      const char* routeName = dccexProtocol.routes.get(i)->getRouteName().c_str();
      int routeId = dccexProtocol.routes.get(i)->getRouteId();
      routeList.addItem(i, routeName, routeId, dummy);
      CONSOLE.print("Route before|after c_str(): ");
      CONSOLE.print(dccexProtocol.routes.get(i)->getRouteName());
      CONSOLE.print(F("|"));
      CONSOLE.println(routeName);
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
      const char* turnoutName = dccexProtocol.turnouts.get(i)->getTurnoutName().c_str();
      int turnoutId = dccexProtocol.turnouts.get(i)->getTurnoutId();
      turnoutList.addItem(i, turnoutName, turnoutId, dummy);
      CONSOLE.print("Turnout before|after c_str(): ");
      CONSOLE.print(dccexProtocol.turnouts.get(i)->getTurnoutName());
      CONSOLE.print(F("|"));
      CONSOLE.println(turnoutName);
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
      const char* turntableName = dccexProtocol.turntables.get(i)->getTurntableName().c_str();
      int turntableId = dccexProtocol.turntables.get(i)->getTurntableId();
      turntableList.addItem(i, turntableName, turntableId, dummy);
      CONSOLE.print("Turntable index ");
      CONSOLE.print(i);
      CONSOLE.print(": ");
      CONSOLE.println(turntableName);
    }
  }
}
