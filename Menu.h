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

/*
This menu class can create a structured parent/subparent menu system,
with paginated menu pages also.
*/

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "defines.h"
#include "DisplayFunctions.h"

// Structure for each menu item - no parameters
struct MenuItem {
  int index;
  const char* label;
  int16_t objectId;
  void (*action)();
  MenuItem* next;

  MenuItem() : index(0), label(nullptr), objectId(0), action(nullptr), next(nullptr) {}

  MenuItem(int idx, const char* label, int16_t objectId, void (*action)()) :
    index(idx), label(label), objectId(objectId), action(action), next(nullptr) {}
};

/*
Menu class
*/
class Menu {
public:
  // Constructor
  Menu(const char* label) {
    this->label = label;
    currentPage = 1;
    parentMenu = nullptr;
  };

  // Public functions
  void addItem(int index, const char* label, int16_t objectId, void (*action)());
  void setParent(Menu* parent);
  void display();
  void handleKeyPress(char key);
  int getItemCount();
  Menu* getParent();
  MenuItem getItem(int index);
  int getSelectedItem();

private:
  // Private variables
  static const int MAX_ITEMS = 40;
  const char* label;
  int currentPage;
  Menu* parentMenu;
  MenuItem* head;
  int selectedItemIndex;

  // Private functions
  void displayMenu();

};

// End of class

extern Menu* currentMenuPtr;

extern Menu rosterList;
extern Menu routeList;
extern Menu turnoutList;
extern Menu turntableList;

void createMenus();
void dummy();

#endif
