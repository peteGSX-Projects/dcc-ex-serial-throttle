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
#include "Menu.h"

/*
Public functions
*/
void Menu::addItem(const char* label, int16_t objectId, void (*action)()) {
  if (itemCount < MAX_ITEMS) {
    items[itemCount].label = label;
    items[itemCount].objectId = objectId;
    items[itemCount].action = action;
    itemCount++;
  }
}

void Menu::setParent(Menu* parent) {
  parentMenu = parent;
}

void Menu::display() {
  displayMenu();
}

void Menu::handleKeyPress(char key){
  switch (key) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
      if (itemCount > 0) {
        int index = key - '1' + (currentPage - 1) * 9;
        if (index < itemCount) {
          items[index].action();
        }
      }
      break;
    case '#':
      if (currentPage < (itemCount / 9) + 1) {
        currentPage++;
        displayMenu();
      }
      break;
    case '*':
      if (parentMenu != nullptr) {
        parentMenu->display();
      }
      break;
    default:
      break;
  }
}

/*
Private functions
*/
void Menu::displayMenu(){
  oled.clear();
  oled.set1X();
  oled.setCursor(0, 0);
  oled.println(label);

  int startIdx = (currentPage - 1) * 9;
  int endIdx = min(startIdx + 9, itemCount);

  for (int i = startIdx; i < endIdx; i++) {
    oled.print(i + 1);
    oled.print(". ");
    oled.println(items[i].label);
  }

  oled.print("Page ");
  oled.println(currentPage);
}

Menu mainMenu("Main Menu");
Menu rosterMenu("Roster Menu");
Menu trackManagerMenu("TrackManager Menu");

void createMenus() {
  rosterMenu.setParent(&mainMenu);
  trackManagerMenu.setParent(&mainMenu);
  mainMenu.addItem("Roster", 0, []() { rosterMenu.display(); });
}
