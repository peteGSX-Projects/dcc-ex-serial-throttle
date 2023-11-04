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

class MenuSystem;

class MenuItemBase {
public:
  /// @brief Type of item for the object
  enum ItemType {
    Normal,
    Action,
    Entry,
    Menu,
    Throttle,
  };

  /// @brief Constructor
  /// @param label 
  /// @param type 
  MenuItemBase(const char* label, MenuItemBase::ItemType type);
  
  /// @brief Get the object's label
  /// @return 
  const char* getLabel() const;

  /// @brief Get the item type of the object
  /// @return 
  ItemType getType();

  /// @brief Set the next object in the list
  /// @param next 
  void setNext(MenuItemBase* next);

  /// @brief Get the next object
  /// @return 
  MenuItemBase* getNext() const;
  
  /// @brief Set the objects index
  /// @param index 
  void setIndex(int index);

  /// @brief Get the index value of this object
  /// @return 
  int getIndex() const;
  
  /// @brief What happens when the item is selected from a menu
  virtual void select(OLED& oled) {};
  
  /// @brief Display method of this object
  /// @param oled 
  virtual void display(OLED& oled) {};
  
  /// @brief Keypad input processing of this object
  /// @param key 
  /// @param keyState 
  virtual void handleKeys(char key, KeyState keyState, OLED& oled) {};
  
  /// @brief Sets the static attribute for referencing the menu system
  /// @param menuSystem 
  static void setMenuSystem(MenuSystem* menuSystem);

  /// @brief Set the parent object
  /// @param parent 
  void setParent(MenuItemBase* parent);

protected:
  const char* _label;
  ItemType _type;
  MenuItemBase* _next;
  int _index;
  MenuItemBase* _parent;
  static MenuSystem* _menuSystem;

  friend class MenuSystem;

};

class ActionMenuItem : public MenuItemBase {
public:
  typedef void (*Action)();
  typedef void (*ActionWithObject)(void*);

  /// @brief Constructor for an action item
  /// @param label 
  /// @param action 
  ActionMenuItem(const char* label, Action action);
  
  /// @brief Constructor for an action item associated with an object
  /// @param label 
  /// @param action 
  /// @param objectPointer 
  ActionMenuItem(const char* label, ActionWithObject action, void* objectPointer);

  /// @brief Execute the associated function when selected
  /// @param oled 
  void select(OLED& oled) override;

private:
  Action _action;
  ActionWithObject _actionWithObject;
  void* _objectPointer;

};

class EntryMenuItem : public MenuItemBase {
public:
  typedef void (*Action)(int number);
  EntryMenuItem(const char* label, const char* instruction, Action action);

  void select(OLED& oled) override;
  void display(OLED& oled) override;
  void handleKeys(char key, KeyState keyState, OLED& oled) override;

private:
  const char* _instruction;
  Action _action;
  int _inputNumber;
  char _inputBuffer[6];
  int _inputIndex;
  int _inputKeyColumn;

};

class Menu : public MenuItemBase {
public:
  Menu(const char* label);
  void select(OLED& oled) override;
  void display(OLED& oled) override;
  void handleKeys(char key, KeyState keyState, OLED& oled) override;
  void addMenuItem(MenuItemBase* item);
  MenuItemBase* getItemAtIndex(int index);
  void setSelectedItem(MenuItemBase* item);

private:
  MenuItemBase* _itemList;
  int _itemCount;
  int _currentPage;
  int _itemsPerPage;

};

class ThrottleScreen : public MenuItemBase {
public:
  ThrottleScreen();
  void display(OLED& oled) override;
  void handleKeys(char key, KeyState keyState, OLED& oled) override;
  void setMenu(MenuItemBase* menu);

private:
  MenuItemBase* _menu;

};

class MenuSystem {
public:
  MenuSystem(OLED& oled);
  void display();
  void handleKeys(char key, KeyState keyState);
  void setHome(MenuItemBase* home);
  void goHome();
  void setCurrentItem(MenuItemBase* currentItem);

private:
  OLED& _oled;
  MenuItemBase* _currentItem;
  MenuItemBase* _home;
  
};

#endif




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