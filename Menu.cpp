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

// Create menus
Menu homeScreen("Home Screen");
Menu mainMenu("Main Menu");
Menu rosterMenu("Roster Menu");
Menu trackManagerMenu("TrackManager Menu");
Menu trackTestMenu("Testing Menu");
Menu rosterList("Roster List");
Menu routeList("Route List");
Menu turnoutList("Turnout List");
Menu turntableList("Turntable List");

Menu* currentMenuPtr = &homeScreen;

/*
Public functions
*/
void Menu::addItem(int index, const char* label, int16_t objectId, void (*action)()) {
  MenuItem* newItem = new MenuItem(index, label, objectId, action);
  if (!head) {
    head = newItem;
  } else {
    newItem->next = head;
    head = newItem;
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
      if (getItemCount() > 0) {
        int index = key - '1' + (currentPage - 1) * 9;
        if (index < getItemCount()) {
          getItem(index).action();
        }
      }
      break;
    case '#':
      if (currentPage < (getItemCount() / 9) + 1) {
        currentPage++;
        // displayMenu();
      } else {
        currentPage = 1;
      }
      displayMenu();
      break;
    case '*':
      if (currentMenuPtr->getParent() == nullptr) {
        currentMenuPtr = &mainMenu;
      } else if (currentMenuPtr->getParent() != nullptr) {
        currentMenuPtr = currentMenuPtr->getParent();
      }
      currentMenuPtr->display();
      break;
    default:
      break;
  }
}

int Menu::getItemCount() {
  int count = 0;
  MenuItem* current = head;
  
  while (current != nullptr) {
    count++;
    current = current->next;
  }

  return count;
}

Menu* Menu::getParent() {
  return parentMenu;
}

MenuItem Menu::getItem(int index) {
  int currentIndex = 0;
  MenuItem* currentItem = head;

  while (currentItem != nullptr) {
    if (currentIndex == index) {
      return *currentItem;
    }
    currentIndex++;
    currentItem = currentItem->next;
  }
  return MenuItem{0, "", 0, nullptr};
}

/*
Private functions
*/
// Display the menu with a single page of options
void Menu::displayMenu(){
  // The top most menu only displays the * key to access the menu system
  if (parentMenu == nullptr) {
    displayHomeScreen();
  } else {
  // All other menus show 10 per page in two columns
    oled.clear();
    oled.set1X();
    oled.setCursor(0, 0);
    oled.print(label);

    int startIdx = (currentPage - 1) * 10;
    int endIdx = min(startIdx + 10, getItemCount());

    // Display number keys 1 -> 0 (instead of 10) to select
    int key = 1;
    int column = 0;
    int row = 1;
    for (int i = startIdx; i < endIdx; i++) {
      oled.setCursor(column, row);
      oled.print(key);
      oled.print(" ");
      oled.print(getItem(i).label);
      key++;
      // If next key would be 10, make it 0
      if (key > 9) {
        key = 0;
      }
      row++;
      // 6th row means row 1 in second column
      if (row > 5) {
        row = 1;
        column = 60;
      }
    }

    oled.setCursor(0, 7);
    oled.print("* Back");
    if (getItemCount() > 10) {
      oled.setCursor(60, 7);
      oled.print("# Next page");
    }
  }
}

/*
Function to create required menus including static list items
*/
void createMenus() {
  // Create menu structure
  mainMenu.setParent(&homeScreen);
  rosterMenu.setParent(&mainMenu);
  trackManagerMenu.setParent(&mainMenu);
  trackTestMenu.setParent(&trackManagerMenu);
  rosterList.setParent(&mainMenu);
  routeList.setParent(&mainMenu);
  turnoutList.setParent(&mainMenu);
  turntableList.setParent(&mainMenu);

  // Create static menu items
  mainMenu.addItem(0, "Testing", 0, []() { rosterMenu.display(); });
  mainMenu.addItem(1, "TrackManager", 0, []() { trackManagerMenu.display(); });
  mainMenu.addItem(2, "Roster", 0, []() { rosterList.display(); });
  mainMenu.addItem(3, "Routes", 0, []() { routeList.display(); });
  mainMenu.addItem(4, "Turnouts", 0, []() { turnoutList.display(); });
  mainMenu.addItem(5, "Turntables", 0, []() { turntableList.display(); });

  rosterMenu.addItem(0, "Test 1", 1, dummy);
  rosterMenu.addItem(1, "Test 2", 2, dummy);
  rosterMenu.addItem(2, "Test 3", 3, dummy);
  rosterMenu.addItem(3, "Test 4", 4, dummy);
  rosterMenu.addItem(4, "Test 5", 5, dummy);
  rosterMenu.addItem(5, "Test 6", 6, dummy);
  rosterMenu.addItem(6, "Test 7", 7, dummy);
  rosterMenu.addItem(7, "Test 8", 8, dummy);
  rosterMenu.addItem(8, "Test 9", 9, dummy);
  rosterMenu.addItem(9, "Test 10", 10, dummy);
  rosterMenu.addItem(10, "Test 11", 11, dummy);
  rosterMenu.addItem(11, "Test 12", 12, dummy);
  trackManagerMenu.addItem(0, "Test Menu", 0, []() { trackTestMenu.display(); });
}

void dummy() {
  // if (currentMenuPtr != nullptr && currentMenuPtr->getItemCount() > 0) {
  //   int16_t objectId = currentMenuPtr->items[0].objectId;
  //   CONSOLE.print("Dummy item ");
  //   CONSOLE.println(objectId);
  // }
}
