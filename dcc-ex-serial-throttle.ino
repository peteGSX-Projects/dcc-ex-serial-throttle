/*

*/

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {

}


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

void setRoute(uint16_t routeID) {

  Serial.print(F("<>"));
}