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

#ifndef THROTTLESETUP_H
#define THROTTLESETUP_H

#include <Arduino.h>

// If number of throttles not defined, define here as 3
#ifndef NUM_THROTTLES
#define NUM_THROTTLES 3
#endif

extern const int numThrottles;

struct DisplayAttributes {
  int startColumn;
  int endColumn;
  int startRow;
  int endRow;
  int textColumn;
  int fontSize;
};

struct ThrottleSetup {
  DisplayAttributes speed;
  DisplayAttributes direction;
  DisplayAttributes adress;
  int potPin;
};

extern ThrottleSetup throttleSetup[NUM_THROTTLES];

#endif