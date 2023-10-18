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

When constructing menus, consider that a 128 x 64 OLED can accommodate:
- 25 characters per row
- 9 rows

Each menu page can accommodate 2 columns of 5 items, meaning the text for each
item is limited to 10 characters as 5 characters are lost to the key for each
item plus space between.
*/

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "defines.h"
#include "DisplayFunctions.h"
#include "Throttle.h"

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
    this->_label = label;
    _currentPage = 1;
    _parentMenu = nullptr;
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
  void setInputMode();
  Menu* addSubmenu(const char* label);

private:
  // Private variables
  const char* _label;
  int _currentPage;
  Menu* _parentMenu;
  MenuItem* _head;
  int _selectedItemIndex;
  bool _inputMode = false;
  char _inputBuffer[5];
  byte _inputIndex = 0;
  bool _isLocoInput = false;

  // Private functions
  void _displayMenu();
  void _doHomeFunctions(char key);

};

// End of class

extern Menu* currentMenuPtr;

extern Menu rosterList;
extern Menu routeList;
extern Menu turnoutList;
extern Menu turntableList;

void createMenus();
void setThrottleContext();
void enterLocoAddress();
void forgetLoco();
void selectFromRoster();
void setLocoFromRoster();
void noAction();    // For menu items that just display info

#endif
