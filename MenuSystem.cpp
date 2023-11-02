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

MenuItem::MenuItem(const char* label) {
  _label=label;
  _next=nullptr;
}

FunctionMenuItem::FunctionMenuItem(const char* label, void (*function)())
  : MenuItem(label), _function(function) {}

Submenu::Submenu(const char* label)
  : MenuItem(label) {}

Menu::Menu(OLED& oled) {
  _oled=oled;
  _itemList=nullptr;
  _itemCount=0;
}

void Menu::addMenuItem(MenuItem* item) {
  if (this->_itemList==nullptr) {
    this->_itemList=item;
  } else {
    MenuItem* current=this->_itemList;
    while (current->_next!=nullptr) {
      current=current->_next;
    }
    current->_next=item;
  }
  _itemCount++;
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