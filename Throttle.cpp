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

/*
Constructor, set input mode on construction
*/
Throttle::Throttle(uint8_t throttleNumber, uint8_t potPin) {
  _potPin = potPin;
  pinMode(_potPin, INPUT);
  _throttleNumber = throttleNumber;
}

/*
Process throttle object:
- Average potentiometer input over samples
- If average has changed, update display and loco speed
*/
void Throttle::process() {
  if (_locoAddress == 0) return;
  uint16_t _instantValue = analogRead(_potPin);
  _sum += _instantValue;
  if (_valueCount == SAMPLES) _sum -= _values[_currentIndex];
  _values[_currentIndex] = _instantValue;
  if (++_currentIndex >= SAMPLES) _currentIndex = 0;
  if (_valueCount < SAMPLES) _valueCount += 1;
  _rollingAverage = _sum/_valueCount;
  _speedChanged = false;
  if (_speed != map(_rollingAverage, POT_MIN, POT_MAX, 0, 126)) {
    _speed = map(_rollingAverage, POT_MIN, POT_MAX, 0, 126);
    _speedChanged = true;
  }
}

/*
Return this throttle's number (for display purposes)
*/
uint8_t Throttle::getThrottleNumber() {
  return _throttleNumber;
}

/*
Associate a loco address with this throttle
Can be a consist instead?
*/
void Throttle::setLocoAddress(uint16_t address) {
  _locoAddress = address;
  
}

/*
Return the current loco address
*/
uint16_t Throttle::getLocoAddress() {
  return _locoAddress;
}

/*
Flag if throttle is a consist or not
*/
bool Throttle::isConsist() {
  return _isConsist;
}

/*Function to flag if the speed has changed
*/
bool Throttle::speedChanged() {
  return _speedChanged;
}

/*
Returns the current speed
*/
uint8_t Throttle::getSpeed() {
  return map(_rollingAverage, POT_MIN, POT_MAX, 0, 126);
}

/*
Forgets the acquired loco
This needs to delete any Loco or Consist objects in use
*/
void Throttle::forgetLoco() {
  _locoAddress = 0;
}

// Create throttle objects
Throttle throttle1(1, POT1_PIN);
Throttle throttle2(2, POT2_PIN);
Throttle throttle3(3, POT3_PIN);
