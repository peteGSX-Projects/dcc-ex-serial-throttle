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
#include "StaticMenus.h"

DCCEXProtocol dccexProtocol(NUM_THROTTLES);
DCCEXCallbacks dccexCallbacks;

bool connectionRequested=false;
bool connected=false;
bool errorDisplayed=false;
uint8_t connectionRetries=CONNECT_RETRIES;
unsigned long retryDelay=1000;
unsigned long lastRetry=0;
bool gotRoster=false;
bool gotRoutes=false;
bool gotTurnouts=false;
bool gotTurntables=false;
int progressX=0;
int progressY=30;

// Function to validate a connection to the CS has been made
void validateConnection() {
  if (!connected) {
    if (!connectionRequested) {
      connectionRequested = true;
      dccexProtocol.sendServerDetailsRequest();
      display.clear();
      display.setFont(DEFAULT_FONT);
      display.drawStr(0, 10, "Connecting to DCC-EX...");
      display.sendBuffer();
    } else if (dccexProtocol.isServerDetailsReceived()) {
      connected = true;
      menuSystem.goHome();
    } else if (millis() - lastRetry > retryDelay && connectionRetries > 0) {
      dccexProtocol.sendServerDetailsRequest();
      lastRetry = millis();
      connectionRetries--;
      display.setFont(DEFAULT_FONT);
      display.setCursor(progressX, progressY);
      display.print(".");
      display.sendBuffer();
      progressX+=5;
      if (progressX>122) {
        progressY+=8;
        progressX=0;
      }
    } else if (connectionRetries == 0 && !errorDisplayed) {
      errorDisplayed = true;
      displayConnectionError();
    }
  }
}

// Function to update roster entries from the CS
// To trigger after startup, simply set requestedRoster to false
void updateRoster() {
  if (dccexProtocol.isRosterFullyReceived() && !gotRoster) {
    gotRoster = true;
    Menu* rMenu=menuSystem.findMenuByLabel("Roster");
    if (!rMenu) return;
    for (Loco* loco=dccexProtocol.roster->getFirst(); loco; loco=loco->getNext()) {
      rMenu->addMenuItem(new ActionMenuItem(loco->getName(), nullptr));
    }
  }
}

// Function to update route entries from the CS
// To trigger after startup, simply set requestedRoutes to false
void updateRoutes() {
  if (dccexProtocol.isRouteListFullyReceived() && !gotRoutes) {
    gotRoutes = true;
    Menu* rtMenu=menuSystem.findMenuByLabel("Routes");
    if (!rtMenu) return;
    for (Route* r=dccexProtocol.routes->getFirst(); r; r=r->getNext()) {
      rtMenu->addMenuItem(new ActionMenuItem(r->getName(), nullptr));
    }
  }
}

// Function to update turnout entries from the CS
// To trigger after startup, simply set requestedTurnouts to false
void updateTurnouts() {
  if (dccexProtocol.isTurnoutListFullyReceived() && !gotTurnouts) {
    gotTurnouts=true;
    Menu* tMenu=menuSystem.findMenuByLabel("Turnouts");
    for (Turnout* t=dccexProtocol.turnouts->getFirst(); t; t=t->getNext()) {
      tMenu->addMenuItem(new ActionMenuItem(t->getName(), toggleTurnout, t));
    }
  }
}

// Function to update turntable entries from the CS
// To trigger after startup, simply set requestedTurntables to false
void updateTurntables() {
  if (dccexProtocol.isTurntableListFullyReceived() && !gotTurntables) {
    gotTurntables=true;
    Menu* ttMenu=menuSystem.findMenuByLabel("Turntables");
    if (!ttMenu) return;
    for (Turntable* tt=dccexProtocol.turntables->getFirst(); tt; tt=tt->getNext()) {
      char* ttName=tt->getName();
      CONSOLE.println(ttName);
      Menu* newTTMenu=new Menu(ttName);
      ttMenu->addMenuItem(newTTMenu);
      for (TurntableIndex* idx=tt->getFirstIndex(); idx; idx=idx->getNextIndex()) {
        char* idxName=idx->getName();
        CONSOLE.println(idxName);
        newTTMenu->addMenuItem(new ActionMenuItem(idxName, rotateTurntable, idx));
      }
    }
  }
}

// Function to toggle a turnout
// If closed, will throw, if thrown, will close
void toggleTurnout() {
  if (!menuSystem.getCurrentActionItem()) return;
  void* object=menuSystem.getCurrentActionItem()->getObjectPointer();
  Turnout* turnout=static_cast<Turnout*>(object);
  if (!turnout) return;
  int turnoutId = turnout->getId();
  dccexProtocol.toggleTurnout(turnoutId);
}

void closeTurnout() {
  if (!menuSystem.getCurrentActionItem()) return;
  void* object=menuSystem.getCurrentActionItem()->getObjectPointer();
  Turnout* turnout=static_cast<Turnout*>(object);
  if (!turnout) return;
  int turnoutId = turnout->getId();
  dccexProtocol.closeTurnout(turnoutId);
}

void throwTurnout() {
  if (!menuSystem.getCurrentActionItem()) return;
  void* object=menuSystem.getCurrentActionItem()->getObjectPointer();
  Turnout* turnout=static_cast<Turnout*>(object);
  if (!turnout) return;
  int turnoutId = turnout->getId();
  dccexProtocol.throwTurnout(turnoutId);
}

// Function to rotate a turntable
void rotateTurntable() {
  if (!menuSystem.getCurrentActionItem()) return;
  CONSOLE.println(F("Got action item"));
  void* object=menuSystem.getCurrentActionItem()->getObjectPointer();
  TurntableIndex* index=static_cast<TurntableIndex*>(object);
  if (!index) return;
  CONSOLE.println(F("Got index"));
  int ttId=index->getTTId();
  int indexId=index->getId();
  CONSOLE.println(indexId);
  dccexProtocol.sendTurntableAction(ttId, indexId, 0);
}

// Function to turn track power on or off
void trackPowerOn() {
  dccexProtocol.sendTrackPower(PowerOn);
}

void trackPowerOff() {
  dccexProtocol.sendTrackPower(PowerOff);
}

// Function to join or unjoin tracks
void setJoinTracks() {
  // if (currentMenuPtr != nullptr) {
  //   int join = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
  //   if (join == 1) {
  //     CONSOLE.println("Would join but not implemented yet");
  //   } else {
  //     CONSOLE.println("Would unjoin but not implemented yet");
  //   }
  // }
}
