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

#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <Arduino.h>
#include "defines.h"
#include "Keypad.h"

class Menu;

class MenuItem {
public:
  enum ItemType {
    Normal,
    Action,
    Entry,
    Throttle,
  };
  MenuItem(const char* label, ItemType type=ItemType::Normal);
  int getIndex() const;
  virtual void execute() {};
private:
  const char* _label;
  ItemType _type;
  MenuItem* _next;
  int _index;

  friend class Menu;

};

class ActionMenuItem : public MenuItem {
public:
  typedef void (*Action)();
  ActionMenuItem(const char* label, Action action=nullptr);

  void execute() override;

private:
  Action _action;

};

class Menu {
public:
  Menu(OLED& oled, const char* label);
  void addMenuItem(MenuItem* item);
  MenuItem* getItemAtIndex(int index);
  void display();
  void handleKeys(char key, KeyState keyState);
private:
  OLED& _oled;
  const char* _label;
  MenuItem* _itemList;
  int _itemCount;
  int _currentPage;
  int _itemsPerPage;
  
};





// class MenuSystem {
// public:
//   MenuSystem(OLED& oled);

//   void setRootMenu(Menu* rootMenu);

//   void navigate(char key, KeyState keyState);

// private:
//   OLED& _oled;
//   Menu* _currentMenu;

// };






/*
class Menu;

class MenuItem {
public:
  /// @brief Constructor - provide index and label
  /// @param label 
  MenuItem(const char* label, Menu* parent);

  virtual void display(OLED& oled);

  Menu* getParent();

private:
  int _index;
  const char* _label;
  MenuItem* _next;
  Menu* _parent;

  friend class Menu;

};

class ActionItem : public MenuItem {
public:
  /// @brief Constructor - provide index, label, pointer to the object to action, and the callback function
  /// @param label 
  /// @param objectPointer 
  /// @param callback 
  ActionItem(const char* label, void *objectPointer, void (*callback)(), Menu* parent);

  void display(OLED& oled) override;

private:
  void* _objectPointer;
  void (*_callback)();

};

class EntryItem : public MenuItem {
public:
  /// @brief Constructor - provide index, label, instruction line, and the callback to send the entered number to
  /// @param label 
  /// @param instruction 
  /// @param callback 
  EntryItem(const char* label, const char* instruction, void (*callback)(), Menu* parent);

  void display(OLED& oled) override;

private:
  const char* _instruction;
  void (*_callback)();

};

class SubmenuItem : public MenuItem {
public:
  SubmenuItem(const char* label, Menu* submenu, Menu* parent);

  Menu* getSubmenu();

  void display(OLED& oled) override;

private:
  Menu* _submenu;

};

class FunctionMenu : public MenuItem {
public:

private:

};

class Menu {
public:
  /// @brief Constructor for menu
  Menu();

  /// @brief Constructor for a submenu
  /// @param parent 
  /// @param index 
  /// @param label 
  Menu(Menu* parent, const char* label);

  /// @brief Add MenuItem object to the list
  /// @param item 
  void addItem(MenuItem* item);

  /// @brief Get count of menu items in the menu
  /// @return 
  int getItemCount();

  /// @brief Display menu
  /// @param oled 
  void display(OLED& oled);

  /// @brief Navigate the menu
  /// @param key 
  /// @param keyState 
  void navigate(char key, KeyState keyState);

private:
  MenuItem* _firstItem;
  Menu* _firstSubmenu;
  int _itemCount;

};

class MenuSystem {
public:
  /// @brief Constructor
  MenuSystem(OLED* oled);

  void display();

  void navigate(char key, KeyState keyState);

  void setMenu(Menu* menu);

  void begin();

private:
  Menu* _currentMenu;
  OLED* _oled;

};
*/

#endif