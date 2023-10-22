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

#include <Arduino.h>
#include "MenuSystem.h"

/***********************************************************************************
Set up our keypad
***********************************************************************************/
char keys[4][3] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[4] = {KEYPAD_PIN2, KEYPAD_PIN7, KEYPAD_PIN6, KEYPAD_PIN4};
byte pin_column[3] = {KEYPAD_PIN3, KEYPAD_PIN1, KEYPAD_PIN5};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 3);

MenuSystem::MenuSystem(SSD1306AsciiSpi &display, Keypad &keypad)
  : display(display), keypad(keypad), currentMenu(nullptr), currentMenuItem(nullptr) {}

void MenuSystem::begin() {
  display.begin(OLED_TYPE, CS_PIN, DC_PIN);
  displayStartupInfo();
  keypad.setDebounceTime(KEYPAD_DEBOUNCE);
  keypad.setHoldTime(KEYPAD_HOLD);
}

void MenuSystem::loop() {
  char key = keypad.getKey();
  _currentKey = key;
  if (keypad.getState() == HOLD) {
    CONSOLE.print(key);
    CONSOLE.println(F(" held"));
  } else if (keypad.getState() == RELEASED && _currentKey == _previousKey) {
    CONSOLE.print(key);
    CONSOLE.println(F(" was held and now released"));
  } else {
    CONSOLE.print(key);
    CONSOLE.println(F(" pressed"));
  }
  _previousKey = _currentKey;
}