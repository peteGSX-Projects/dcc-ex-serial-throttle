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

MenuSystem menuSystem(display);

void createMenus() {
  ThrottleScreen* throttle=new ThrottleScreen();
  menuSystem.setHome(throttle);
  Menu* home=new Menu("Home");
  throttle->setMenu(home);
  char label[15];
  home->addMenuItem(new ActionMenuItem("Test D1", dummyAction1));
  home->addMenuItem(new ActionMenuItem("Test D2", dummyAction2));
  home->addMenuItem(new EntryMenuItem("Entry", "Instruction", nullptr));
  for (int i=0; i<15; i++) {
    sprintf(label, "Test %d", i);
    home->addMenuItem(new MenuItemBase(label, MenuItemBase::Normal));
  }
  Menu* submenu=new Menu("Submenu");
  home->addMenuItem(submenu);
  for (int i=0; i<15; i++) {
    sprintf(label, "STest %d", i);
    submenu->addMenuItem(new MenuItemBase(label, MenuItemBase::Normal));
  }
}

void dummyAction1() {
  CONSOLE.println("Selected dummy action 1");
}

void dummyAction2() {
  CONSOLE.println("Selected dummy action 2");
}