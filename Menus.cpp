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
#include "Menus.h"

MenuSystem menuSystem(&oled);

Menu throttleScreen("Throttle Screen");
Menu mainMenu(&throttleScreen, 0, "Main Menu");
Menu throttleSetup(&mainMenu, 0, "Throttles");
Menu turnoutList(&mainMenu, 1, "Turnouts");
Menu turntableList(&mainMenu, 2, "Turntables");
Menu rosterList(&mainMenu, 3, "Roster");
