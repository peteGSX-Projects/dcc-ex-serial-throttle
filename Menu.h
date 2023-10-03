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

// Structure for each menu item
struct MenuItem {
  const char* label;
  int16_t objectId;
  void (*action)();
};

/*
Menu class
*/
class Menu {
public:
  // Constructor
  Menu(const char* label) {
    this->label = label;
    itemCount = 0;
    currentPage = 1;
    parentMenu = nullptr;
  };

  // Public functions
  void addItem(const char* label, int16_t objectId, void (*action)());
  void setParent(Menu* parent);
  void display();
  void handleKeyPress(char key);
  int getItemCount();
  Menu* getParent();
  MenuItem getItem(int index);

private:
  // Private variables
  static const int MAX_ITEMS = 40;
  const char* label;
  MenuItem items[MAX_ITEMS];
  int itemCount;
  int currentPage;
  Menu* parentMenu;

  // Private functions
  void displayMenu();

};

extern Menu* currentMenuPtr;

void createMenus();
void dummy();

#endif
