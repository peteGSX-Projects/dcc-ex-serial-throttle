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
#include "ThrottleSetup.h"

// Fonts in use
#define DEFAULT_FONT u8g2_font_NokiaSmallPlain_tr
#define MENU_TITLE_FONT u8g2_font_5x8_mr
#define MENU_ITEM_FONT u8g2_font_5x8_mr
#define SPEED_FONT u8g2_font_spleen12x24_mn
#define DIRECTION_FONT u8g2_font_6x10_mr
#define ADDRESS_FONT u8g2_font_6x10_mr
#define ESTOP_FONT u8g2_font_6x12_mr

extern OLED display;

void displayStartupInfo();
void displayKey(char key, KeyState keyState);
void displayConnectionError();

/// @brief Display speed for the specified throttle
/// @param throttle 
/// @param speed 
void displayThrottleSpeed(int throttle, int speed, bool isHome);

/// @brief Display direction for the specified throttle
/// @param throttle 
/// @param direction 
void displayThrottleDirection(int throttle, Direction direction);

/// @brief Dislay address for the specified throttle
/// @param throttle 
/// @param address 
/// @param isOverridden 
/// @param isConsist 
void displayThrottleAddress(int throttle, int address, bool isOverridden, bool isConsist);

/// @brief Display EStop for the specified throttle
/// @param throttle 
void displayThrottleEStop(int throttle);

#endif
