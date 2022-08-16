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

#include "Arduino.h"
#include "AnalogueAverage.h"

AnalogueAverage::AnalogueAverage(uint8_t pin):_pin(pin) {
  pinMode(_pin, INPUT);
}

void AnalogueAverage::averageInput() {
  uint16_t _instantValue = analogRead(_pin);
  _sum += _instantValue;
  if (_valueCount == SAMPLES) _sum -= _values[_currentIndex];
  _values[_currentIndex] = _instantValue;
  if (++_currentIndex >= SAMPLES) _currentIndex = 0;
  if (_valueCount < SAMPLES) _valueCount += 1;
  _rollingAverage = _sum/_valueCount;
}

uint16_t AnalogueAverage::getAverage() {
  return _rollingAverage;
}