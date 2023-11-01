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
// Public functions
MenuItem::MenuItem(int index, const char* label) {
  _index=index;
  _label=label;
  _next=nullptr;
}

// class ActionItem
ActionItem::ActionItem(int index, const char* label, void *objectPointer, void (*callback)())
  : MenuItem(index, label), _objectPointer(objectPointer), _callback(callback) {}

// class EntryItem
EntryItem::EntryItem(int index, const char* label, const char* instruction, void(*callback)())
  : MenuItem(index, label), _instruction(instruction), _callback(callback) {}

// class SubmenuItem
SubmenuItem::SubmenuItem(int index, const char* label, Menu* submenu)
  : MenuItem(index, label), _submenu(submenu) {}

Menu* SubmenuItem::getSubmenu() {
  return _submenu;
}

// class Menu
// Public functions
Menu::Menu(const char* label) {
  _label=label;
  _firstItem=nullptr;
  _itemCount=0;
}

Menu::Menu(Menu* parent, int index, const char* label) {
  SubmenuItem* newItem=new SubmenuItem(index, label, this);
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
}

// class MenuSystem
MenuSystem::MenuSystem(OLED* oled) {
  _oled=oled;
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

void _displayMainMenu(OLED& oled) {

}

void _displayMenu(OLED& oled, Menu* menu) {

}

void _displayEntry(OLED& oled, EntryItem* entry) {

}