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

Menu* currentMenuPtr;

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
  currentMenuPtr = this;
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
      if (currentMenuPtr->getParent() != nullptr) {
        currentMenuPtr = currentMenuPtr->getParent();
        currentMenuPtr->display();
      }
      break;
    default:
      break;
  }
}

Menu* Menu::getParent() {
  return parentMenu;
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
Menu trackTestMenu("Testing");

void createMenus() {
  rosterMenu.setParent(&mainMenu);
  trackManagerMenu.setParent(&mainMenu);
  trackTestMenu.setParent(&trackManagerMenu);
  mainMenu.addItem("Roster", 0, []() { rosterMenu.display(); });
  mainMenu.addItem("TrackManager", 0, []() { trackManagerMenu.display(); });
  rosterMenu.addItem("Test 1", 1, dummy);
  rosterMenu.addItem("Test 2", 1, dummy);
  rosterMenu.addItem("Test 3", 1, dummy);
  rosterMenu.addItem("Test 4", 1, dummy);
  rosterMenu.addItem("Test 5", 1, dummy);
  rosterMenu.addItem("Test 6", 1, dummy);
  rosterMenu.addItem("Test 7", 1, dummy);
  rosterMenu.addItem("Test 8", 1, dummy);
  rosterMenu.addItem("Test 9", 1, dummy);
  rosterMenu.addItem("Test 10", 1, dummy);
  rosterMenu.addItem("Test 11", 1, dummy);
  rosterMenu.addItem("Test 12", 1, dummy);
  trackManagerMenu.addItem("Test Menu", 0, []() { trackTestMenu.display(); });
}

void dummy() {
  CONSOLE.println("Dummy");
}