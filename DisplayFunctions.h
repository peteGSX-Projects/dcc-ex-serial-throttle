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

#ifdef USE_OLED_SPI
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiSpi.h"
extern SSD1306AsciiSpi oled;
#endif
#ifdef USE_OLED_I2C
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
extern SSD1306AsciiAvrI2c oled;
#endif

void displayStartupInfo();
void displayHomeScreen();
void displayConnectionError();
void updateLoco1Speed();
void updateLoco1Direction();
void updateLoco2Address();
void updateLoco2Speed();
void updateLoco2Direction();
void updateLoco2Address();
void updateLoco3Speed();
void updateLoco3Direction();
void updateLoco3Address();

// void displayEStop();

#endif
