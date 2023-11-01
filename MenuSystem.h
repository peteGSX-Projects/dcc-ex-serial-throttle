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
  /// @brief Constructor - provide index and label
  /// @param index 
  /// @param label 
  MenuItem(int index, const char* label);

private:
  int _index;
  const char* _label;
  MenuItem* _next;

  friend class Menu;

};

class ActionItem : public MenuItem {
public:
  /// @brief Constructor - provide index, label, pointer to the object to action, and the callback function
  /// @param index 
  /// @param label 
  /// @param objectPointer 
  /// @param callback 
  ActionItem(int index, const char* label, void *objectPointer, void (*callback)());

private:
  void* _objectPointer;
  void (*_callback)();

};

class EntryItem : public MenuItem {
public:
  /// @brief Constructor - provide index, label, instruction line, and the callback to send the entered number to
  /// @param index 
  /// @param label 
  /// @param instruction 
  /// @param callback 
  EntryItem(int index, const char* label, const char* instruction, void (*callback)());

private:
  const char* _instruction;
  void (*_callback)();

};

class SubmenuItem : public MenuItem {
public:
  SubmenuItem(int index, const char* label, Menu* submenu);

  Menu* getSubmenu();

private:
  Menu* _submenu;

};

class Menu {
public:
  /// @brief Constructor for menu
  /// @param label 
  Menu(const char* label);

  /// @brief Constructor for a submenu
  /// @param parent 
  /// @param index 
  /// @param label 
  Menu(Menu* parent, int index, const char* label);

  /// @brief Add MenuItem object to the list
  /// @param item 
  void addItem(MenuItem* item);

  /// @brief Get count of menu items in the menu
  /// @return 
  int getItemCount();

  void display(OLED& oled);

  void navigate(char key, KeyState keyState);

private:
  const char* _label;
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

private:
  Menu* _currentMenu;
  OLED* _oled;

};

#endif