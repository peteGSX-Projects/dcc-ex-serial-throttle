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

// #include <Arduino.h>
// #include "defines.h"
// #include "DisplayFunctions.h"
// #include "Throttle.h"
// #include "Keypad.h"

/*
enum MenuItemType {
  MENU,
  ACTION,
  ENTRY,
};

// Structure for each menu item - no parameters
struct MenuItem {
  int index;
  const char* label;
  MenuItemType menuItemType;
  void *objectPointer;
  void (*callback)();
  MenuItem* next;

  MenuItem() : index(0), label(nullptr), menuItemType(MENU), objectPointer(nullptr), callback(nullptr), next(nullptr) {}

  MenuItem(int idx, const char* label, MenuItemType menuItemType, void *objectPointer, void (*callback)()) :
    index(idx), label(label), menuItemType(menuItemType), objectPointer(objectPointer), callback(callback), next(nullptr) {}
};

// Menu class
class Menu {
public:
  // Constructor
  Menu(const char* label, Menu* parent) {
    this->_label = label;
    _currentPage = 1;
    _parentMenu = parent;
  };

  // Public functions
  /// @brief Add a menu
  /// @param index Index of the menu, determins display order
  /// @param label Label of the menu
  /// @param objectPointer Pointer to the menu object
  void addMenu(int index, const char* label, void *objectPointer);

  /// @brief Add an item to a menu that triggers an action
  /// @param index Index of the item, determines display order
  /// @param label Label of the item for display
  /// @param objectPointer Pointer to a DCC-EX object (if appropriate, otherwise nullptr)
  /// @param callback Function to call when menu item selected
  void addActionItem(int index, const char* label, void *objectPointer, void (*callback)());

  /// @brief Add an item to a menu that requires user entry
  /// @param index Index of the item, determines display order
  /// @param label Label of the item for display
  /// @param callback Function to call when menu item selected
  void addEntryItem(int index, const char* label, void (*callback)());

  /// @brief set the parent menu of the menu item
  /// @param parent 
  void setParent(Menu* parent);
  
  /// @brief display the menu
  void display();
  
  /// @brief process the pressed key and key state
  /// @param key 
  void handleKeyPress(char key, KeyState keyState);
  
  /// @brief get the number of menu items associated with the menu
  /// @return 
  int getItemCount();
  
  /// @brief get parent menu object of the item
  /// @return 
  Menu* getParent();
  
  /// @brief get the menu item at the specified index
  /// @param index 
  /// @return 
  MenuItem getItem(int index);
  
  /// @brief get the current selected item's index
  /// @return 
  int getSelectedItem();
  
  /// @brief set flag that numeric input is required
  void setInputMode();

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
  void _addItem(MenuItem* newItem);
  void _displayMenu();
  void _doHomeFunctions(char key);

};

// End of class

extern Menu* currentMenuPtr;

extern Menu rosterList;
extern Menu routeList;
extern Menu turnoutList;
extern Menu turntableList;

void createStaticMenus();

void setThrottleContext();
void enterLocoAddress();
void forgetLoco();
void selectFromRoster();
void setLocoFromRoster();
void noAction();    // For menu items that just display info
*/

#endif
