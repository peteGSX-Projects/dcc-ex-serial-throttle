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

class AnalogueAverage {
  public:
    AnalogueAverage(uint8_t pin, uint8_t samples);
    void averageInput();
    uint16_t getAverage();

  private:
    uint8_t _pin;
    uint8_t _samples;

};

#endif