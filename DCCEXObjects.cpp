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
uint8_t connectionRetries = CONNECT_RETRIES;
unsigned long retryDelay = 1000;
unsigned long lastRetry = 0;
bool gotRoster = false;
bool gotRoutes = false;
bool gotTurnouts = false;
bool gotTurntables = false;

/*
Function to validate a connection to the CS has been made
*/
void validateConnection() {
  if (!connected) {
    if (!connectionRequested) {
      connectionRequested = true;
      dccexProtocol.sendServerDetailsRequest();
      oled.clear();
      oled.setCursor(0, 0);
      oled.print("Connecting to DCC-EX");
      oled.setCursor(0, 2);
    } else if (dccexProtocol.isServerDetailsReceived()) {
      connected = true;
      currentMenuPtr->display();
    } else if (millis() - lastRetry > retryDelay && connectionRetries > 0) {
      dccexProtocol.sendServerDetailsRequest();
      lastRetry = millis();
      connectionRetries--;
      oled.print(".");
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
  if (dccexProtocol.isRosterFullyReceived() && !gotRoster) {
    gotRoster = true;
    for (int i = 0; i < dccexProtocol.roster.size(); i++) {
      Loco* loco = dccexProtocol.roster.get(i);
      rosterList.addItem(i, loco->getLocoName(), loco->getLocoAddress(), setLocoFromRoster);
    }
  }
}

/*
Function to update route entries from the CS
To trigger after startup, simply set requestedRoutes to false
*/
void updateRoutes() {
  if (dccexProtocol.isRouteListFullyReceived() && !gotRoutes) {
    gotRoutes = true;
    for (int i = 0; i < dccexProtocol.routes.size(); i++) {
      Route* route = dccexProtocol.routes.get(i);
      routeList.addItem(i, route->getRouteName(), route->getRouteId(), noAction);
    }
  }
}

/*
Function to update turnout entries from the CS
To trigger after startup, simply set requestedTurnouts to false
*/
void updateTurnouts() {
  if (dccexProtocol.isTurnoutListFullyReceived() && !gotTurnouts) {
    gotTurnouts = true;
    for (int i = 0; i < dccexProtocol.turnouts.size(); i++) {
      Turnout* turnout = dccexProtocol.turnouts.get(i);
      turnoutList.addItem(i, turnout->getTurnoutName(), turnout->getTurnoutId(), toggleTurnout);
    }
  }
}

/*
Function to update turntable entries from the CS
To trigger after startup, simply set requestedTurntables to false
*/
void updateTurntables() {
  if (dccexProtocol.isTurntableListFullyReceived() && !gotTurntables) {
    gotTurntables = true;
    for (int i = 0; i < dccexProtocol.turntables.size(); i++) {
      Turntable* turntable = dccexProtocol.turntables.get(i);
      turntableList.addItem(i, turntable->getTurntableName(), turntable->getTurntableId(), noAction);
    }
  }
}

/*
Function to toggle a turnout
If closed, will throw, if thrown, will close
*/
void toggleTurnout() {
  if (currentMenuPtr != nullptr) {
    int16_t objectId = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
    Turnout* turnout = dccexProtocol.getTurnoutById(objectId);
    if (turnout->getTurnoutState() == TurnoutClosed) {
      dccexProtocol.sendTurnoutAction(objectId, TurnoutThrow);
    } else {
      dccexProtocol.sendTurnoutAction(objectId, TurnoutClose);
    }
  }
}

/*
Function to turn track power on or off
*/
void setTrackPower() {
  if (currentMenuPtr != nullptr) {
    int state = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
    if (state == 1) {
      dccexProtocol.sendTrackPower(PowerOn);
    } else {
      dccexProtocol.sendTrackPower(PowerOff);
    }
  }
}

/*
Function to join or unjoin tracks
*/
void setJoinTracks() {
  if (currentMenuPtr != nullptr) {
    int join = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
    if (join == 1) {
      CONSOLE.println("Would join but not implemented yet");
    } else {
      CONSOLE.println("Would unjoin but not implemented yet");
    }
  }
}
