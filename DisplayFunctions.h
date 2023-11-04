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

#ifndef DISPLAYFUNCTIONS_H
#define DISPLAYFUNCTIONS_H

#include <Arduino.h>
#include "defines.h"
#include "version.h"
#include "Throttle.h"
#include "Keypad.h"

extern OLED display;

void displayStartupInfo();
void displayKey(char key, KeyState keyState);
// void displayHomeScreen();
// void displayConnectionError();
// void displayThrottle1Speed();
// void displayThrottle1Direction();
// void displayThrottle2Address();
// void displayThrottle2Speed();
// void displayThrottle2Direction();
// void displayThrottle2Address();
// void displayThrottle3Speed();
// void displayThrottle3Direction();
// void displayThrottle3Address();

// void displayEStop();

#endif
