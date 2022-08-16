/*
 *  © 2022 Peter Cole
 *
 *  This file is for reading a moving average of an analog input.
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

#ifndef ANALOGAVERAGE_H
#define ANALOGAVERAGE_H

#include "Arduino.h"

#ifndef SAMPLES
#define SAMPLES 20
#endif

class AnalogueAverage {
  public:
    AnalogueAverage(uint8_t pin);
    void averageInput();
    uint16_t getAverage();

  private:
    uint8_t _pin;
    uint8_t _currentIndex = 0;
    uint8_t _valueCount = 0;
    uint16_t _sum = 0;
    uint16_t _values[SAMPLES];
    uint16_t _rollingAverage = 0;

};

#endif