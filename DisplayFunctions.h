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
#define DEFAULT_FONT u8g2_font_NokiaSmallPlain_tf
#define DCCEX_FONT u8g2_font_12x6LED_tf
#define MENU_TITLE_FONT u8g2_font_5x8_tf
#define MENU_ITEM_FONT u8g2_font_5x8_tf
#define SPEED_FONT u8g2_font_NokiaSmallPlain_tf
#define DIRECTION_FONT u8g2_font_5x8_tf
#define ADDRESS_FONT u8g2_font_5x8_tf
#define ESTOP_FONT u8g2_font_5x8_tf

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
