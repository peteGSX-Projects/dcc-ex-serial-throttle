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
#include "MenuSystem.h"

// class MenuItem
MenuItem::MenuItem(const char* label, ItemType type) : _type(type) {
  _label=strdup(label);
  _next=nullptr;
  _index=-1;
}

int MenuItem::getIndex() const {
  return _index;
}

// class ActionMenuItem
ActionMenuItem::ActionMenuItem(const char* label, Action action)
  : MenuItem(label, ItemType::Action) {
    _action=action;
  }

void ActionMenuItem::execute() {
  if (_action) {
    _action();
  }
}

// class EntryMenuItem
EntryMenuItem::EntryMenuItem(const char* label, const char* instruction, Action action)
  : MenuItem(label, ItemType::Entry), _instruction(instruction), _action(action), _inputNumber(0) {
}

void EntryMenuItem::display(OLED& oled) {
  oled.clear();
  oled.set1X();
  oled.setCursor(0, 0);
  oled.print(_label);
  oled.setCursor(0, 1);
  oled.print(_instruction);
  oled.setCursor(0, 3);
  oled.print("#####");
  oled.setCursor(0, 7);
  oled.print("* Back");
  oled.setCursor(70, 7);
  oled.print("# Confirm");
}

void EntryMenuItem::handleKeys(char key, KeyState keyState) {
  if (keyState==PRESSED) {
    
  }
}

// class Menu
Menu::Menu(OLED& oled, const char* label)
  : _oled(oled), _label(label) {
  _itemList=nullptr;
  _itemCount=0;
  _currentPage=0;
  _itemsPerPage=10;
}

void Menu::addMenuItem(MenuItem* item) {
  CONSOLE.println(item->_label);
  if (this->_itemList==nullptr) {
    this->_itemList=item;
  } else {
    MenuItem* current=this->_itemList;
    while (current->_next!=nullptr) {
      current=current->_next;
    }
    current->_next=item;
  }
  item->_index=_itemCount;
  _itemCount++;
}

MenuItem* Menu::getItemAtIndex(int index) {
  for (MenuItem* item=_itemList; item; item=item->_next) {
    if (item->getIndex()==index) {
      return item;
    }
  }
  return nullptr;
}

void Menu::display() {

    int startIndex=_currentPage*_itemsPerPage;
    int endIndex=min(startIndex+_itemsPerPage, _itemCount);
    
    _oled.clear();
    _oled.setFont(OLED_FONT);
    _oled.setCursor(0, 0);
    _oled.print(_label);
    int i=0;
    int column=0;
    int row=1;
    for (int index=startIndex; index<endIndex; index++) {
      MenuItem* item=getItemAtIndex(index);
      
      _oled.setCursor(column, row);
      _oled.print(i);
      _oled.print(F(" "));
      _oled.print(item->_label);
      // If next key would be 10, make it 0
      row++;
      i++;
      // 6th row means row 1 in second column
      if (row > 5) {
        row = 1;
        column = 65;
      }
    }
    _oled.setCursor(0, 7);
    _oled.print(F("* Menu"));
    if (_itemCount>_itemsPerPage) {
      _oled.setCursor(70, 7);
      _oled.print(F("# Page "));
      int nextPage=(_currentPage+1)%((int)ceil(_itemCount/(float)_itemsPerPage))+1;
      _oled.print(nextPage);
    }
}

void Menu::handleKeys(char key, KeyState keyState) {
  if (keyState==PRESSED) {
    switch (key) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        int itemIndex=key-'0';
        itemIndex+=_currentPage*_itemsPerPage;
        MenuItem* item=getItemAtIndex(itemIndex);
        if (item) {
          if (item->_type==MenuItem::Action) {
            item->execute();
          } else {
            CONSOLE.print(F("Selected item "));
            CONSOLE.println(itemIndex);
          }
        }
        break;
      }
      
      case '#':
        _currentPage=(_currentPage+1) % ((int)ceil(_itemCount/(float)_itemsPerPage));
        display();
        break;
      
      default:
        break;
    }
  }
}

void Menu::setSelectedItem(MenuItem* item) {
  _selectedItem=item;
  display();
}



/*
// class MenuItem
// Public functions
MenuItem::MenuItem(const char* label, Menu* parent) {
  _label=label;
  _parent=parent;
  _next=nullptr;
}

Menu* MenuItem::getParent() {
  return _parent;
}

// class ActionItem
ActionItem::ActionItem(const char* label, void *objectPointer, void (*callback)(), Menu* parent)
  : MenuItem(label, parent), _objectPointer(objectPointer), _callback(callback) {}

// class EntryItem
EntryItem::EntryItem(const char* label, const char* instruction, void(*callback)(), Menu* parent)
  : MenuItem(label, parent), _instruction(instruction), _callback(callback) {}

// class SubmenuItem
SubmenuItem::SubmenuItem(const char* label, Menu* submenu, Menu* parent)
  : MenuItem(label, parent), _submenu(submenu) {}

Menu* SubmenuItem::getSubmenu() {
  return _submenu;
}

void SubmenuItem::display(OLED& oled) {
  oled.clear();
  oled.setFont(OLED_FONT);
  oled.setCursor(0, 0);
  oled.print("Menu");
}

// class Menu
// Public functions
Menu::Menu() {
  _firstItem=nullptr;
  _itemCount=0;
}

Menu::Menu(Menu* parent, const char* label) {
  SubmenuItem* newItem=new SubmenuItem(label, this, parent);
  parent->addItem(newItem);
}

void Menu::addItem(MenuItem* item) {
  if (this->_firstItem==nullptr) {
    this->_firstItem=item;
  } else {
    MenuItem* current=this->_firstItem;
    while (current->_next!=nullptr) {
      current=current->_next;
    }
    current->_next=item;
  }
  _itemCount++;
}

int Menu::getItemCount() {
  return _itemCount;
}

void Menu::display(OLED& oled) {
  MenuItem* item=_firstItem;
  item->display(oled);
}

void Menu::navigate(char key, KeyState keyState) {

}

// class MenuSystem
MenuSystem::MenuSystem(OLED* oled) {
  _oled=oled;
  _currentMenu=nullptr;
}

void MenuSystem::display() {
  _currentMenu->display(*_oled);
}

void MenuSystem::navigate(char key, KeyState keyState) {
  _currentMenu->navigate(key, keyState);
}

void MenuSystem::setMenu(Menu* menu) {
  _currentMenu=menu;
}

void MenuSystem::begin() {
  
}
*/