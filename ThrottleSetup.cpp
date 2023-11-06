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

#include "ThrottleSetup.h"

ThrottleSetup throttleSetup[NUM_THROTTLES]={
  { // Throttle 0
    {0, 45, 0, 2, 4},  // Speed display attributes
    {0, 51, 3, 4, 10}, // Direction display attributes
    {0, 42, 5, 5, 0}, // Address display attributes
    PA0,  // Pot pin
  },
  { // Throttle 1
    {46, 87, 0, 2, 46},
    {52, 93, 3, 4, 52},
    {43, 86, 5, 5, 43},
    PA1,
  },
  { // Throttle 2
    {88, 128, 0, 2, 88},
    {94, 128, 3, 4, 94},
    {87, 128, 5, 5, 87},
    PA2,
  },
};
