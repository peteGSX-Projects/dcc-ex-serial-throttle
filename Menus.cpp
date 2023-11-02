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

// MenuSystem menuSystem(oled);

Menu mainMenu(oled, "Main Menu");

void createMenus() {
  // for (int i=0; i<10; i++) {
  //   char *label;
  //   sprintf(label, "Test %d", i);
  //   mainMenu.addMenuItem(new MenuItem(label));
  // }
  mainMenu.addMenuItem(new MenuItem("Test 1"));
  mainMenu.addMenuItem(new MenuItem("Test 2"));
  
}