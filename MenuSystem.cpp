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

MenuSystem* MenuItemBase::_menuSystem=nullptr;

// class MenuItem
MenuItemBase::MenuItemBase(const char* label, MenuItemBase::ItemType type)
  : _type(type) {
  _label=strdup(label);
  _parent=nullptr;
  _next=nullptr;
  _index=-1;
}

const char* MenuItemBase::getLabel() const {
  return _label;
}

MenuItemBase::ItemType MenuItemBase::getType() {
  return _type;
}

void MenuItemBase::setNext(MenuItemBase* next) {
  _next=next;
}

MenuItemBase* MenuItemBase::getNext() const {
  return _next;
}

void MenuItemBase::setIndex(int index) {
  _index=index;
}

int MenuItemBase::getIndex() const {
  return _index;
}

void MenuItemBase::setMenuSystem(MenuSystem* menuSystem) {
  _menuSystem=menuSystem;
}

void MenuItemBase::setParent(MenuItemBase* parent) {
  _parent=parent;
}

// class ActionMenuItem
ActionMenuItem::ActionMenuItem(const char* label, Action action)
  : MenuItemBase(label, ItemType::Action) {
    _action=action;
  }

void ActionMenuItem::execute() {
  if (_action) {
    _action();
  }
}

// class EntryMenuItem
EntryMenuItem::EntryMenuItem(const char* label, const char* instruction, Action action)
  : MenuItemBase(label, ItemType::Entry), _instruction(instruction), _action(action), _inputNumber(0) {
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

void EntryMenuItem::handleKeys(char key, KeyState keyState, OLED& oled) {
  if (keyState==PRESSED) {
    switch(key) {
      case '*':
        if (_parent) {
          _menuSystem->setCurrentItem(_parent);
          _parent->display(oled);
        }
        break;
      
      case '#':
        break;
      
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        break;
      
      default:
        break;
    }
  }
}

// class Menu
Menu::Menu(const char* label)
  : MenuItemBase(label, MenuItemBase::Menu) {
  _itemList=nullptr;
  _itemCount=0;
  _currentPage=0;
  _itemsPerPage=10;
}

void Menu::display(OLED& oled) {
  int startIndex=_currentPage*_itemsPerPage;
  int endIndex=min(startIndex+_itemsPerPage, _itemCount);
  
  oled.clear();
  oled.setFont(OLED_FONT);
  oled.setCursor(0, 0);
  oled.print(_label);
  int i=0;
  int column=0;
  int row=1;
  for (int index=startIndex; index<endIndex; index++) {
    MenuItemBase* item=getItemAtIndex(index);
    
    oled.setCursor(column, row);
    oled.print(i);
    oled.print(F(" "));
    oled.print(item->getLabel());
    // If next key would be 10, make it 0
    row++;
    i++;
    // 6th row means row 1 in second column
    if (row > 5) {
      row = 1;
      column = 65;
    }
  }
  oled.setCursor(0, 7);
  oled.print(F("* Back"));
  if (_itemCount>_itemsPerPage) {
    oled.setCursor(70, 7);
    oled.print(F("# Page "));
    int nextPage=(_currentPage+1)%((int)ceil(_itemCount/(float)_itemsPerPage))+1;
    oled.print(nextPage);
  }
}

void Menu::handleKeys(char key, KeyState keyState, OLED& oled) {
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
        MenuItemBase* item=getItemAtIndex(itemIndex);
        if (item) {
          MenuItemBase::ItemType type=item->getType();
          switch (type) {
            case MenuItemBase::Action:
              item->execute();
              break;

            case MenuItemBase::Menu:
            case MenuItemBase::Entry:
              _menuSystem->setCurrentItem(item);
              item->display(oled);
              break;

            default:
              CONSOLE.print(F("Selected item "));
              CONSOLE.println(itemIndex);
              break;
          }
        }
        break;
      }

      case '*':
        if (this->_parent) {
          _menuSystem->setCurrentItem(this->_parent);
          _parent->display(oled);
        }
        break;
      
      case '#':
        _currentPage=(_currentPage+1) % ((int)ceil(_itemCount/(float)_itemsPerPage));
        display(oled);
        break;
      
      default:
        break;
    }
  }
}

void Menu::addMenuItem(MenuItemBase* item) {
  CONSOLE.println(item->getLabel());
  if (this->_itemList==nullptr) {
    this->_itemList=item;
  } else {
    MenuItemBase* current=this->_itemList;
    while (current->getNext()!=nullptr) {
      current=current->getNext();
    }
    current->setNext(item);
  }
  item->setIndex(_itemCount);
  item->setParent(this);
  _itemCount++;
}

MenuItemBase* Menu::getItemAtIndex(int index) {
  for (MenuItemBase* item=_itemList; item; item=item->getNext()) {
    if (item->getIndex()==index) {
      return item;
    }
  }
  return nullptr;
}

// class ThrottleScreen
ThrottleScreen::ThrottleScreen()
  : MenuItemBase(nullptr, MenuItemBase::Throttle) {}

void ThrottleScreen::display(OLED& oled) {
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("Throttles here"));
  oled.setCursor(0, 7);
  oled.print(F("* Menu"));
}

void ThrottleScreen::handleKeys(char key, KeyState keyState, OLED& oled) {
  if (_menu) {
    _menuSystem->setCurrentItem(_menu);
    _menu->display(oled);
  }
}

void ThrottleScreen::setMenu(MenuItemBase* menu) {
  _menu=menu;
  _menu->setParent(this);
}

// class MenuSystem
MenuSystem::MenuSystem(OLED& oled)
  : _oled(oled) {
    _currentItem=nullptr;
    _home=nullptr;
  }

void MenuSystem::display() {
  if (_currentItem) {
    _currentItem->display(_oled);
  }
}

void MenuSystem::handleKeys(char key, KeyState keyState) {
  if (_currentItem) {
    _currentItem->handleKeys(key, keyState, _oled);
  }
}

void MenuSystem::setHome(MenuItemBase* home) {
  _home=home;
  _currentItem=home;
  MenuItemBase::setMenuSystem(this);
}

void MenuSystem::goHome() {
  _currentItem=_home;
  _home->display(_oled);
}

void MenuSystem::setCurrentItem(MenuItemBase* currentItem) {
  _currentItem=currentItem;
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