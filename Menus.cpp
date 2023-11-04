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
  Menu* mainMenu=new Menu("Main Menu");
  throttle->setMenu(mainMenu);
  Menu* throttleList=new Menu("Throttles");
  mainMenu->addMenuItem(throttleList);
  char label[11];
  for (int i=1; i<4; i++) {
    sprintf(label, "Throttle %d", i);
    Menu* throttleMenu=new Menu(label);
    throttleList->addMenuItem(throttleMenu);
    throttleMenu->addMenuItem(new ActionMenuItem("Select from roster", nullptr, nullptr));
    throttleMenu->addMenuItem(new EntryMenuItem("Enter address", "DCC address:", nullptr));
    throttleMenu->addMenuItem(new ActionMenuItem("Remove loco", nullptr, nullptr));
    throttleMenu->addMenuItem(new ActionMenuItem("Display consist", nullptr, nullptr));
    throttleMenu->addMenuItem(new ActionMenuItem("Forget loco/consist", nullptr, nullptr));
    CONSOLE.println(throttleMenu->getLabel());
  }
  mainMenu->addMenuItem(new Menu("Turnouts"));
  mainMenu->addMenuItem(new Menu("Routes"));
  mainMenu->addMenuItem(new Menu("Turntables"));
  mainMenu->addMenuItem(new Menu("Roster"));
  Menu* tracksMenu=new Menu("Tracks");
  mainMenu->addMenuItem(tracksMenu);
  tracksMenu->addMenuItem(new ActionMenuItem("Power On", nullptr));
  tracksMenu->addMenuItem(new ActionMenuItem("Power Off", nullptr));
  tracksMenu->addMenuItem(new ActionMenuItem("Join", nullptr));
  tracksMenu->addMenuItem(new ActionMenuItem("Unjoin", nullptr));
  Menu* trackManager=new Menu("TrackManager");
  tracksMenu->addMenuItem(trackManager);
}

void dummyAction1() {
  CONSOLE.println("Selected dummy action 1");
}

void dummyAction2() {
  CONSOLE.println("Selected dummy action 2");
}