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

#include "HelperFunctions.h"

/*
Function to check if provided char array is null-terminated and, if not, make it so
*/
const char* nullTerminatedCharArray(const char* checkArray) {
  if (checkArray[strlen(checkArray) - 1] == '0') {
    return checkArray;
  } else {
    char* checkArrayCopy = new char[strlen(checkArray) + 1];
    strcpy(checkArrayCopy, checkArray);
    return checkArrayCopy;
  }
}