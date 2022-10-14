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
  // Set our parameter type flags for easy reference
  const uint8_t TBD = 0;
  const uint8_t INTEGER = 1;
  const uint8_t TEXT = 2;
  const uint8_t STRING = 3;
  
  // Ensure we accept at least 10 params/objects, and set the limit
  uint8_t maxObjects = MAX_OBJECTS;
  if (MAX_OBJECTS < 10) {
    maxObjects = 10;
  }

  // Struct for our parameters, they can be different types
  struct {
    uint8_t type;
    int16_t integer;
    byte text;
    String string;
  } params[maxObjects];

  byte opcode = responseBytes[0];   // Our OPCODE is always the first parameter
  byte state = 1;                   // This lets us progress through splitting stages
  bool buildString = false;         // Flag for building string params "..."
  uint8_t parameterCount = 0;       // Count of params
  byte runningValue = 0;            // Placeholder to build the param
  bool terminated = false;          // Flag for when we receive the terminator '\0'
  const byte *remainingParams = responseBytes + 1;  // Skip OPCODE
  uint8_t paramType = TBD;          // Flag for the parameter type we're dealing with

/*
Flags/data:
  data type
  parameter number
  how do we build a parameter across multiple bytes?

-> byte
Start while loop
Is byte a space ' '?
  Yes -> skip to next byte, it's a separator
  No -> continue processing
Is byte a terminator '\0'?
  Yes -> end parsing
  No -> continue processing







If ' ' skip it (space delimited)
If '\0' that's the end, exit switch and loop
If '"' it's the start of a string, everything until the next '"' is the same string including ' '
Everything else forms part of the parameter
eg. 

<JR>:
<jR 2004 2006 2010>

<JR 2004>:
<jR 2004 "QR 2004" "Light/Horn/Bell/F4">

Throttle broadcast:
<l 2004 0 179 0>

<JT>:
<jT 101 102>

<JT 101>:
<jT 101 C "Turnout 1">
*/
  while (parameterCount < maxObjects && terminated == false) {
    byte nextByte = *remainingParams;
    Serial.print(F("Building param "));
    Serial.print(parameterCount);
    Serial.print(F(" with byte value "));
    Serial.println(nextByte);
    switch (state) {
      case 1:
        // If it's a space, skip it
        if (nextByte == ' ' && buildString == false) {
          Serial.println("Space");
          runningValue = 0;
          paramType = TBD;
          break;
        }
        // If it's the termination character, end parsing
        if (nextByte == '\0') {
          Serial.println(F("Terminated"));
          terminated = true;
          break;
        }
        state = 2;
        continue;
      case 2:
        // Build our parameter
        // If " and not building a string, flag to do so and get next byte
        if (nextByte == '"' && buildString == false) {
          Serial.println(F("String start"));
          paramType = STRING;
          buildString = true;
          state = 1;
          break;
        }
        // If " and building a string, flag the end of the string
        if (nextByte == '"' && buildString == true) {
          Serial.println(F("String end"));
          buildString = false;
        }
        if (paramType == STRING) {
          Serial.print(F("String append: "));
          runningValue = runningValue + nextByte;
          params[parameterCount].type = STRING;
          params[parameterCount].string = String(runningValue);
          Serial.println((String) runningValue);
          state = 1;
          break;
        }
        if (paramType == TBD && nextByte == '-') {
          Serial.println(F("Negative number"));
          paramType = INTEGER;
          runningValue = nextByte;
          state = 1;
          break;
        }
        if (paramType == INTEGER || (paramType == TBD && nextByte >= '0' && nextByte <= '9')) {
          Serial.print(F("Number: "));
          paramType = INTEGER;
          runningValue = runningValue + nextByte;
          params[parameterCount].type = INTEGER;
          params[parameterCount].integer = int(runningValue);
          Serial.println(int(runningValue));
          state = 1;
          break;
        }
        if (paramType == TBD) {
          Serial.print(F("Text: "));
          params[parameterCount].type = TEXT;
          params[parameterCount].text = nextByte;
          Serial.println((char) nextByte);
        }
        parameterCount++;
        state = 1;
    }
    remainingParams++;
  }

  switch(opcode) {
    case 'j':
      Serial.println(F("Throttle response"));
      Serial.print(F("Found "));
      Serial.print(parameterCount);
      Serial.println(" params");
      for (uint16_t i = 0; i < parameterCount; i++) {
        Serial.print(F("Param number "));
        Serial.print(i);
        Serial.print(F(" type is "));
        Serial.println(params[i].type);
        switch (params[i].type) {
          case INTEGER:
            Serial.println(int(params[i].integer));
            break;
          case TEXT:
            Serial.println(params[i].text);
            break;
          case STRING:
            Serial.println(String(params[i].string));
            break;
        }
      }
      break;
    default:
      break;
  }
};

/*
Command OPCODE list:
p - track power
r - read loco on programming track
l - cab broadcast
j - throttle commands
H - turnout throw/close
q/Q - defined sensor changes
*/