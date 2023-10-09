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

#ifndef THROTTLE_H
#define THROTTLE_H

#include <Arduino.h>
#include "defines.h"
#include "DCCEXObjects.h"

const uint8_t SAMPLES = SLIDER_SAMPLES;

struct LocoNode {
  Loco* loco;
  LocoNode* next;
};

class Throttle {
public:
  // Constructor
  Throttle(uint8_t throttleNumber, uint8_t potPin, LocoNode* initialLocoList);

  void process();
  uint8_t getThrottleNumber();
  void setLocoAddress(uint16_t address, LocoSource source);
  uint16_t getLocoAddress();
  bool speedChanged();
  uint8_t getSpeed();
  bool isConsist();
  void forgetLoco();
  void setDirection(bool direction);
  bool getDirection();
  bool isOverridden();
  bool addressInUse(uint16_t address);

private:
  uint8_t _potPin;  // pin the potentiometer is on for this throttle
  uint8_t _currentIndex = 0;
  uint8_t _valueCount = 0;
  uint16_t _sum = 0;
  uint16_t _values[SAMPLES];
  uint8_t _speed = 0;
  uint16_t _rollingAverage = 0;
  bool _speedChanged = false;
  uint16_t _locoAddress = 0;
  bool _isConsist = false;
  uint8_t _throttleNumber;
  bool _direction = 0;
  bool _isOverridden = false;
  LocoNode* _locoList = nullptr;  // Linked list containing Locos

};

extern Throttle throttle1;
extern Throttle throttle2;
extern Throttle throttle3;

#endif
