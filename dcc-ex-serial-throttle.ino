/*

*/

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {

}

void setTrackPower(bool powerState) {
  Serial.print(F("<"));
  Serial.print(powerState);
  Serial.print(">");
}

void joinProgMain() {
  Serial.print(F("<1 JOIN>"));
}

void throttleLoco(uint16_t locoAddress, int8_t speed, bool direction) {
  if (locoAddress < 1 || locoAddress > 10293) {
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

void forgetAllLocos() {
  Serial.print(F("<->")):
}

void forgetLoco(uint16_t locoAddress) {
  if (locoAddress < 1 || locoAddress > 10293) {
    return;
  }
  Serial.print(F("<- "));
  Serial.print(locoAddress);
  Serial.print(F(">"));
}

void eStopAll() {
  Serial.print(F("<!>"));
}

void setLocoFunction(uint16_t locoAddress, uint8_t function, bool state) {
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

void accActivateLinear(uint16_t linearAddress, bool state) {
  if (linearAddress > 2044) {
    return;
  }
  Serial.print(F("<a "));
  Serial.print(linearAddress);
  Serial.print(F(" "));
  Serial.print(state);
  Serial.print(F(">"));
}

void accActivateAddr(uint16_t address, uint8_t subaddress, bool state) {
  if (address > 511 || subaddress > 3) {
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

void getTurnouts() {
  Serial.print(F("<T>"));
}

void setTurnout(uint16_t turnoutID, bool position) {
  if (turnoutID > 32767) {
    return;
  }
  Serial.print(F("<T "));
  Serial.print(turnoutID);
  Serial.print(F(" "));
  Serial.print(position);
  Serial.print(F(">"));
}
