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

// Constructor, set input mode on construction
Throttle::Throttle(int throttleNumber, /*LocoNode* initialLocoList*/ Loco* loco, int dtPin, int clkPin, int swPin,
            ThrottleCallback_t singleClickCallback,
            ThrottleCallback_t doubleClickCallback,
            ThrottleCallback_t longPressCallback)
  : _encoder(dtPin, clkPin), _button(swPin), //_throttleNumber(throttleNumber),
    _singleClickCallback(singleClickCallback),
    _doubleClickCallback(doubleClickCallback),
    _longPressCallback(longPressCallback) {
  // _locoList=initialLocoList;
  _loco=nullptr;
  _throttleNumber=throttleNumber;

  _button.setSingleClickCallback(_buttonSingleClickCallback, this);
  _button.setDoubleClickCallback(_buttonDoubleClickCallback, this);
  _button.setLongPressCallback(_buttonLongPressCallback, this);
}

// Process throttle object:
void Throttle::process() {
  // if (_locoAddress == 0) return;
  if (!_loco) return;
  int speed=_loco->getSpeed();
  unsigned char result=_encoder.process();
  // if (result==DIR_CW && _speed<126) {
  if (result==DIR_CW && speed<126) {
    // _speed++;
    speed++;
    _speedChanged=true;
  // } else if (result==DIR_CCW && _speed>0) {
  } else if (result==DIR_CCW && speed>0) {
    // _speed--;
    speed--;
    _speedChanged=true;
  } else {
    _speedChanged=false;
  }
  if (_speedChanged==true) {
    // dccexProtocol.setThrottle(_throttleNumber, _speed, _direction);
    dccexProtocol.setThrottle(_loco, speed, _loco->getDirection());
  }
  _button.poll();

}

// Return this throttle's number (for display purposes)
int Throttle::getThrottleNumber() {
  return _throttleNumber;
}

/*
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
  dccexProtocol.setThrottle(_throttleNumber, _speed, _direction);
}
*/

void Throttle::setLoco(Loco* loco) {
  if (_loco && _loco->getSource()==LocoSource::LocoSourceEntry) {
    delete _loco;
  }
  _loco=loco;
}

// Return the current loco address
int Throttle::getLocoAddress() {
  // return _locoAddress;
  if (!_loco) return 0;
  return _loco->getAddress();
}

/*
// Flag if throttle is a consist or not
bool Throttle::isConsist() {
  return (dccexProtocol.throttle[_throttleNumber].getLocoCount()>1) ? true : false;
}
*/

// Function to flag if the speed has changed
// Sends new speed to the CS also
bool Throttle::speedChanged() {
  return _speedChanged;
}

void Throttle::setSpeedChanged() {
  _speedChanged=true;
}

// Returns the current speed
int Throttle::getSpeed() {
  // return _speed;
  if (!_loco) return 0;
  return _loco->getSpeed();
}

// Forgets the acquired loco
// This needs to delete any Loco or Consist objects in use
void Throttle::forgetLoco(int address) {
  /*
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
  */
  if (_loco && _loco->getSource()==LocoSource::LocoSourceEntry) {
    delete _loco;
  }
  _loco=nullptr;
}

// Sets the direction if speed = 0
// 1 = reverse
// 0 = forward
// Sends the change to the CS also
void Throttle::setDirection(Direction direction){
  // if (_speed > 0) return;
  // _direction = direction;
  // dccexProtocol.setThrottle(_throttleNumber, _speed, _direction);
  if (!_loco || _loco->getSpeed()>0) return;
  dccexProtocol.setThrottle(_loco, _loco->getSpeed(), direction);
}

// Get current throttle direction
// 1 = reverse
// 0 = forward
Direction Throttle::getDirection() {
  // return _direction;
  if (!_loco) return Direction::Forward;
  return _loco->getDirection();
}

void Throttle::setDirectionChanged() {
  _directionChanged=true;
}

bool Throttle::directionChanged() {
  return _directionChanged;
}

// Function to check if the specified address is in use by this throttle
// By design, checks consists as well
bool Throttle::addressInUse(int address) {
  /*
  LocoNode* currentNode=_locoList;
  while (currentNode!=nullptr) {
    if (currentNode->loco->getAddress()==address) {
      return true;
    }
    currentNode=currentNode->next;
  }
  return false;
  */
  if (!_loco || _loco->getAddress()!=address) return false;
  return true;
}

Throttle* Throttle::findThrottleByLoco(Throttle** throttleArray, int numThrottles, Loco* loco) {
  for (int i=0; i<numThrottles; ++i) {
    if (throttleArray[i]!=nullptr && throttleArray[i]->_loco == loco) {
      return throttleArray[i];
    }
  }
  return nullptr;
}

void Throttle::_buttonSingleClickCallback(void* param) {
  if (param) {
    Throttle* th=static_cast<Throttle*>(param);
    th->_handleSingleClick();
  }
}

void Throttle::_handleSingleClick() {
  if (_singleClickCallback) {
    _singleClickCallback(_throttleNumber);
  }
}

void Throttle::_buttonDoubleClickCallback(void* param) {
  if (param) {
    Throttle* th=static_cast<Throttle*>(param);
    th->_handleDoubleClick();
  }
}

void Throttle::_handleDoubleClick() {
  if (_doubleClickCallback) {
    _doubleClickCallback(_throttleNumber);
  }
}

void Throttle::_buttonLongPressCallback(void* param) {
  if (param) {
    Throttle* th=static_cast<Throttle*>(param);
    th->_handleLongPress();
  }
}

void Throttle::_handleLongPress() {
  if (_longPressCallback) {
    _longPressCallback(_throttleNumber);
  }
}
