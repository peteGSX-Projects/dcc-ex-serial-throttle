/*
 *  © 2022 Peter Cole
 *
 *  This file is part of the DCC-EX API library for Arduino.
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
 *  along with Turntable-EX.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include "dcc-ex-api.h"

void setTrackPower(bool powerState) {
/*
This function sets track power, applies to both Main and Prog.
powerState = 0 (off) or 1 (on)
*/
  Serial.print(F("<"));
  Serial.print(powerState);
  Serial.print(">");
}

void setJoin() {
/*
This function joins Prog to Main, and will turn track power on.
Any programming activities or track power off will unjoin again.
*/
  Serial.print(F("<1 JOIN>"));
}

void setLocoThrottle(uint16_t locoAddress, int8_t speed, bool direction) {
/*
This function controls the speed and direction for a locomotive.
locoAddress = any valid DCC decoder address
speed = -1 (emergency stop) to 126, with 0 being a normal stop
direction = 0 (reverse) or 1 (forward)
*/
  if (locoAddress == 0 || locoAddress > 10293) {
    return;
  }
  if (speed < -1 || speed > 126) {
    return;
  }
  Serial.print(F("<t 1 "));
  Serial.print(locoAddress);
  Serial.print(F(" "));
  Serial.print(speed);
  Serial.print(F(" "));
  Serial.print(direction);
  Serial.print(F(">"));
}

void setForgetAllLocos() {
/*
This function tells the throttle to forget all locomotives.
*/
  Serial.print(F("<->"));
}

void setForgetLoco(uint16_t locoAddress) {
/*
This functions tells the throttle to forget a specific locomotive.
locoAddress = any valid DCC decoder address
*/
  if (locoAddress < 1 || locoAddress > 10293) {
    return;
  }
  Serial.print(F("<- "));
  Serial.print(locoAddress);
  Serial.print(F(">"));
}

void setEstopAll() {
/*
This function emergency stops all locomotives.
*/
  Serial.print(F("<!>"));
}

void setLocoFunction(uint16_t locoAddress, uint8_t function, bool state) {
/*
This function sets the state of locomotive functions.
locoAddress = any valid DCC decoder address
function = 0 to 68
state = 0 (off) or 1 (on)
*/
  if (locoAddress < 1 || locoAddress > 10293) {
    return;
  }
  if (function > 68) {
    return;
  }
  Serial.print(F("<F "));
  Serial.print(locoAddress);
  Serial.print(F(" "));
  Serial.print(function);
  Serial.print(F(" "));
  Serial.print(state);
  Serial.print(F(">"));
}

void setAccActivateLinear(uint16_t linearAddress, bool state) {
/*
This function is used to activate/deactive a DCC accessory decoder via it's linear address.
linearAddress = 1 to 2044
state = 0 (deactivate) or 1 (activate)
*/
  if (linearAddress == 0 || linearAddress > 2044) {
    return;
  }
  Serial.print(F("<a "));
  Serial.print(linearAddress);
  Serial.print(F(" "));
  Serial.print(state);
  Serial.print(F(">"));
}

void setAccActivateAddr(uint16_t address, uint8_t subaddress, bool state) {
/*
This function is used to activate/deactive a DCC accessory decoder via it's address and subaddress.
address = 1 to 511
subaddress = 0 to 3
state = 0 (deactivate) or 1 (activate)
*/
  if (address == 0 || address > 511 || subaddress > 3) {
    return;
  }
  Serial.print(F("<a "));
  Serial.print(address);
  Serial.print(F(" "));
  Serial.print(subaddress);
  Serial.print(F(" "));
  Serial.print(state);
  Serial.print(F(">"));
}

void getAllTurnouts() {
/*
This function returns an array/struct? of turnouts that can be controlled.
*/
  Serial.print(F("<JT>"));
  /* Returns <jT id1 id2 id3 ...> THIS WILL NOT WORK RIGHT NOW
  for (uint8_t i; i < numIDs; i++) {
    getTurnout(i);
    // Returns <jT id state "[description]">
  }
  */
}

void getTurnout(uint16_t turnoutID) {
/*
This function returns the ID, state, and description of the specified turnout ID.
<jT id state "description">
turnoutID = 1 to 32767
*/
  if (turnoutID == 0 || turnoutID > 32767) {
    return;
  }
  Serial.print(F("<JT "));
  Serial.print(turnoutID);
  Serial.print(F(">"));
}

void setTurnout(uint16_t turnoutID, bool position) {
/*
This function closes or throws the specified turnout.
turnoutID = 1 to 32767
position = 0 (closed) or 1 (thrown)
*/
  if (turnoutID == 0 || turnoutID > 32767) {
    return;
  }
  Serial.print(F("<T "));
  Serial.print(turnoutID);
  Serial.print(F(" "));
  Serial.print(position);
  Serial.print(F(">"));
}

void getAllRoutes() {
/*
This function returns an array/struct? of all defined route and automation IDs.
*/
  Serial.print(F("<JA>"));
  /* Returns <jA id1 id2 id3 ...> THIS WILL NOT WORK RIGHT NOW
  for (uint8_t i; i < numIDs; i++) {
    getRoute(i);
    // Returns <jA id type "[description]">
  }
  */
}

void getRoute(uint16_t routeID) {
/* This function returns the ID, type, and description of the specific route/automation.
<jA id type "description">
routeID = 1 to 32767
*/
  if (routeID == 0 || routeID > 32767) {
    return;
  }
  Serial.print(F("<JA "));
  Serial.print(routeID);
  Serial.print(F(">"));
}

void setStartRoute(uint16_t locoAddress, uint16_t routeID) {
/*
This function starts the specified route or automation.
routeID = 1 to 32767
*/
  if (locoAddress == 0 or locoAddress > 10293) {
    return;
  }
  if (routeID == 0 || routeID > 32767) {
    return;
  }
  Serial.print(F("</ START "));
  Serial.print(locoAddress);
  Serial.print(F(" "));
  Serial.print(routeID);
  Serial.print(F(">"));
}

void getRosterEntries() {
/*
This function returns an array/struct? of all defined roster entries.
*/
  Serial.print(F("<JR>"));
  /* Returns <jR id1 id2 id3 ...> THIS WILL NOT WORK RIGHT NOW
  for (uint8_t i; i < numIDs; i++) {
    getRoster(i);
    // Returns <jR id type "[description]">
  }
  */
};

void getRoster(uint16_t rosterID) {
/*
This function returns the ID, description, and function map of the specified roster entry.
<jR id "description" "function1/function2/function3/...">
rosterID = 1 to 32767
*/
  if (rosterID == 0 || rosterID > 32767) {
    return;
  }
  Serial.print(F("<JR "));
  Serial.print(rosterID);
  Serial.print(F(">"));
};

void parseAPIResponse(byte* responseBytes, uint8_t arraySize) {
/*
This function parses input received in array of chars according to the API reference
(https://dcc-ex.com/reference/developers/api.html)
commandChars[0] = OPCODE
commandChars[1] onwards are parameters, each separated by a space, first parameter is non-blank
*/
  const uint8_t maxParams = 10;
  char opcode = responseBytes[0];
  int16_t params[maxParams];
  byte state = 1;
  byte parameterCount = 0;
  int16_t runningValue = 0;
  const byte *remainingParams = responseBytes + 1;
  bool signNegative = false;

  for (int16_t i = 0; i < maxParams; i++) {
    params[i] = 0;
  }

  while (parameterCount < maxParams) {
    byte hot = *remainingParams;
    // switch (state) {
    //   case 1: // skipping spaces before a param
    //         if (hot == ' ')
    //             break;
    //         if (hot == '\0' || hot == '>')
    //             // return parameterCount;
    //             break;
    //         state = 2;
    //         continue;

    //     case 2: // checking sign
    //         signNegative = false;
    //         runningValue = 0;
    //         state = 3;
    //         if (hot != '-')
    //             continue;
    //         signNegative = true;
    //         break;
    //     case 3: // building a parameter
    //         if (hot >= '0' && hot <= '9')
    //         {
    //             runningValue = (usehex?16:10) * runningValue + (hot - '0');
    //             break;
    //         }
    //         if (hot >= 'a' && hot <= 'z') hot=hot-'a'+'A'; // uppercase a..z
    //         if (usehex && hot>='A' && hot<='F') {
    //             // treat A..F as hex not keyword
    //             runningValue = 16 * runningValue + (hot - 'A' + 10);
    //             break;
    //         }
    //         if (hot=='_' || (hot >= 'A' && hot <= 'Z'))
    //         {
    //             // Since JMRI got modified to send keywords in some rare cases, we need this
    //             // Super Kluge to turn keywords into a hash value that can be recognised later
    //             runningValue = ((runningValue << 5) + runningValue) ^ hot;
    //             break;
    //         }
    //         responseBytes[parameterCount] = runningValue * (signNegative ? -1 : 1);
    //         parameterCount++;
    //         state = 1;
    //         continue;
    //     }
    //     remainingParams++;

    // }
  }

  // switch(opcode) {
  //   case 'j':
  //     Serial.println(F("Throttle response"));
  //     break;
  //   default:
  //     break;
  // }
};

/*
Command OPCODE list:
p - track power
r - read loco on programming track
l - cab broadcast
j - throttle commands
*/