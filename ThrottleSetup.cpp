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
    {4, 30},
    {10, 40},
    {0, 50},
    PA0,  // Pot pin
  },
  { // Throttle 1
    {46, 30},
    {52, 40},
    {43, 50},
    PA1,
  },
  { // Throttle 2
    {88, 30},
    {94, 40},
    {87, 50},
    PA2,
  },
};
