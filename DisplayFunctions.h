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

// Fonts in use
#define DEFAULT_FONT u8g2_font_NokiaSmallPlain_tf
#define DCCEX_FONT u8g2_font_12x6LED_tf
#define ANIMALS u8g2_font_unifont_t_animals
#define EMOJIS u8g2_font_unifont_t_emoticons

const int EmojiSadFace=0x0055;
const int AnimalCat=0x0028;

extern OLED display;

void displayStartupInfo();
void displayKey(char key, KeyState keyState);
void displayConnectionError();

#endif
