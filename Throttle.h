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
#include "DCCEXObjects.h"

const uint8_t SAMPLES = 10;

class Throttle {
public:
  // Constructor
  Throttle(uint8_t potPin);

  void process();
  void setLocoAddress(int16_t address);

private:
  uint8_t _potPin;  // pin the potentiometer is on for this throttle
  uint8_t _currentIndex = 0;
  uint8_t _valueCount = 0;
  uint16_t _sum = 0;
  uint16_t _values[SAMPLES];
  uint16_t _rollingAverage = 0;

};

#endif
