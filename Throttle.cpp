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

#include "Throttle.h"
#include "defines.h"

// Constructor, set input mode on construction
Throttle::Throttle(int throttleNumber, int potPin, LocoNode* initialLocoList, OLED& oled)
  : _oled(oled) {
  _potPin=potPin;
  pinMode(_potPin, INPUT);
  _throttleNumber=throttleNumber;
  _locoList=initialLocoList;
}

// Process throttle object:
// - Average potentiometer input over samples
// - If average has changed, update display and loco speed
void Throttle::process() {
  if (_locoAddress == 0) return;
  int _instantValue = analogRead(_potPin);
  _sum += _instantValue;
  if (_valueCount == SLIDER_SAMPLES) _sum -= _values[_currentIndex];
  _values[_currentIndex] = _instantValue;
  if (++_currentIndex >= SLIDER_SAMPLES) _currentIndex = 0;
  if (_valueCount < SLIDER_SAMPLES) _valueCount += 1;
  _rollingAverage = _sum/_valueCount;
  _speedChanged = false;
  if (_speed != map(_rollingAverage, POT_MIN, POT_MAX, 0, 126)) {
    _speed = map(_rollingAverage, POT_MIN, POT_MAX, 0, 126);
    _speedChanged = true;
    if (dccexProtocol.throttle[_throttleNumber].getLocoCount() > 0) {
      dccexProtocol.sendThrottleAction(_throttleNumber, _speed, _getDirectionName(_direction));
    }
  }
}

// Return this throttle's number (for display purposes)
int Throttle::getThrottleNumber() {
  return _throttleNumber;
}

// Associate a loco object with this throttle
// Sends the initial speed and direction to the CS also
void Throttle::setLocoAddress(int address, LocoSource source) {
  if (_locoAddress > 0) this->forgetLoco(_locoAddress);
  _locoAddress = address;
  if (addressInUse(_locoAddress)) return;
  for (int i=0; i<NUM_THROTTLES; i++) {
    
  }
  Loco* newLoco = new Loco(address, source);
  LocoNode* newNode = new LocoNode;
  newNode->loco = newLoco;
  newNode->next = nullptr;

  if (_locoList == nullptr) {
    _locoList = newNode;
  } else {
    LocoNode* currentNode = _locoList;
    while (currentNode->next != nullptr) {
      currentNode = currentNode->next;
    }
    currentNode->next = newNode;
  }
  dccexProtocol.throttle[_throttleNumber].addFromEntry(_locoAddress, FacingForward);
  dccexProtocol.sendThrottleAction(_throttleNumber, _speed, _getDirectionName(_direction));
}

// Return the current loco address
int Throttle::getLocoAddress() {
  return _locoAddress;
}

// Flag if throttle is a consist or not
bool Throttle::isConsist() {
  return (dccexProtocol.throttle[_throttleNumber].getLocoCount()>1) ? true : false;
}

// Function to flag if the speed has changed
// Sends new speed to the CS also
bool Throttle::speedChanged() {
  return _speedChanged;
}

// Returns the current speed
int Throttle::getSpeed() {
  return map(_rollingAverage, POT_MIN, POT_MAX, 0, 126);
}

// Forgets the acquired loco
// This needs to delete any Loco or Consist objects in use
void Throttle::forgetLoco(int address) {
  LocoNode* previousNode = nullptr;
  LocoNode* currentNode = _locoList;

  while (currentNode != nullptr) {
    if (currentNode->loco->getAddress() == address) {
      if (previousNode == nullptr) {
        _locoList = currentNode->next;
      } else {
        previousNode->next = currentNode->next;
      }
      delete currentNode->loco;
      delete currentNode;
      _locoAddress = 0;
      return; // Exit the function after successfully removing the node
    }
    previousNode = currentNode;
    currentNode = currentNode->next;
  }
}

// Sets the direction if speed = 0
// 1 = reverse
// 0 = forward
// Sends the change to the CS also
void Throttle::setDirection(bool direction){
  if (_speed > 0) return;
  _direction = direction;
  dccexProtocol.sendThrottleAction(_throttleNumber, _speed, _getDirectionName(_direction));
}

// Get current throttle direction
// 1 = reverse
// 0 = forward
bool Throttle::getDirection() {
  return _direction;
}

// True if throttle speed and/or direction has been overridden by another throttle
bool Throttle::isOverridden() {
  return _isOverridden;
}

// Function to check if the specified address is in use by this throttle
// By design, checks consists as well
bool Throttle::addressInUse(int address) {
  LocoNode* currentNode=_locoList;
  while (currentNode!=nullptr) {
    if (currentNode->loco->getAddress()==address) {
      return true;
    }
    currentNode=currentNode->next;
  }
  return false;
}

void Throttle::displaySpeed() {
  displayThrottleSpeed(_throttleNumber, _speed);
}

void Throttle::displayDirection() {
  displayThrottleDirection(_throttleNumber, _getDirectionName(_direction));
}

void Throttle::displayAddress() {
  displayThrottleAddress(_throttleNumber, _locoAddress, _isOverridden, isConsist());
}

void Throttle::displayEStop() {
  displayThrottleEStop(_throttleNumber);
}

// Helper function to convert direction bool to Direction char
Direction Throttle::_getDirectionName(bool direction) {
  return (direction) ? Forward : Reverse;
}
