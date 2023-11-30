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
#include "StaticMenus.h"

// MenuSystem menuSystem(display);
MenuSystem menuSystem;

void createMenus() {
  ThrottleScreen* throttle=new ThrottleScreen();
  menuSystem.setHome(throttle);
  Menu* mainMenu=new Menu("Main Menu");
  throttle->setMenu(mainMenu);
  Menu* throttleList=new Menu("Throttles");
  mainMenu->addMenuItem(throttleList);
  mainMenu->addMenuItem(new Menu("Turnouts"));
  mainMenu->addMenuItem(new Menu("Routes"));
  mainMenu->addMenuItem(new Menu("Turntables"));
  mainMenu->addMenuItem(new Menu("Roster"));
  Menu* tracksMenu=new Menu("Tracks");
  mainMenu->addMenuItem(tracksMenu);
  char label[25];
  for (int i=0; i<NUM_THROTTLES; i++) {
    sprintf(label, "Throttle %d", i+1);
    Menu* throttleMenu=new ThrottleMenu(label, i);
    throttleList->addMenuItem(throttleMenu);
    sprintf(label, "Add to %d from roster", i+1);
    throttleMenu->addMenuItem(new Menu(label));
    throttleMenu->addMenuItem(new EntryMenuItem("Enter address", "DCC address:", nullptr));
    throttleMenu->addMenuItem(new ActionMenuItem("Read address", readLocoAddress));
    throttleMenu->addMenuItem(new ActionMenuItem("Forget loco/consist", forgetLoco));
    // sprintf(label, "Throttle %d consist", i+1);
    // Menu* consistMenu=new ThrottleMenu(label, i);
    // throttleMenu->addMenuItem(consistMenu);
    // sprintf(label, "Roster %d +", i+1);
    // consistMenu->addMenuItem(new Menu(label));
    // consistMenu->addMenuItem(new EntryMenuItem("Enter +", "DCC address:", nullptr));
    // consistMenu->addMenuItem(new ActionMenuItem("Read", readLocoAddress));
    // consistMenu->addMenuItem(new ActionMenuItem("Members", nullptr));
    // consistMenu->addMenuItem(new ActionMenuItem("Loco -", nullptr));
    // consistMenu->addMenuItem(new ActionMenuItem("Forget", forgetLoco));
    // consistMenu->addMenuItem(new ActionMenuItem("Loco <", nullptr));
    // consistMenu->addMenuItem(new ActionMenuItem("Loco >", nullptr));
    // consistMenu->addMenuItem(new ActionMenuItem("Loco Rev", nullptr));
  }
  tracksMenu->addMenuItem(new ActionMenuItem("Power On", trackPowerOn));
  tracksMenu->addMenuItem(new ActionMenuItem("Power Off", trackPowerOff));
  tracksMenu->addMenuItem(new ActionMenuItem("Join", nullptr));
  tracksMenu->addMenuItem(new ActionMenuItem("Unjoin", nullptr));
  Menu* trackManager=new Menu("TrackManager");
  tracksMenu->addMenuItem(trackManager);
}
