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
  _parent=nullptr;
  _next=nullptr;
  _index=-1;
  _label=strdup(label);
}

char* MenuItemBase::getLabel() const {
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

MenuItemBase::ObjectType MenuItemBase::getObjectType() {
  return _objectType;
}

int MenuItemBase::getObjectId() {
  return _objectId;
}

// class ActionMenuItem
ActionMenuItem::ActionMenuItem(const char* label, Action action, void* objectPointer)
  : MenuItemBase(label, MenuItemBase::Action) {
    _action=action;
    _objectPointer=objectPointer;
  }

void ActionMenuItem::select(OLED& oled) {
  _menuSystem->setCurrentActionItem(this);
  if (_action) {
    _action();
  }
}

void* ActionMenuItem::getObjectPointer() {
  return _objectPointer;
}

// class EntryMenuItem
EntryMenuItem::EntryMenuItem(const char* label, const char* instruction, Action action)
  : MenuItemBase(label, ItemType::Entry), _instruction(instruction), _action(action), _inputNumber(0) {
  _inputIndex=0;
  _inputKeyColumn=0;
}

void EntryMenuItem::select(OLED& oled) {
  _menuSystem->setCurrentItem(this);
  _inputIndex=0;
  _inputKeyColumn=0;
  this->display(oled);
}

void EntryMenuItem::display(OLED& oled) {
  oled.clear();
  oled.setFont(MENU_TITLE_FONT);
  oled.setCursor(0, 6);
  oled.print(_label);
  oled.drawHLine(0, 7, 128);
  oled.setCursor(0, 17);
  oled.print(_instruction);
  oled.setCursor(0, 26);
  oled.print("#####");
  oled.drawHLine(0, 54, 128);
  oled.setCursor(0, 63);
  oled.print("* Back");
  oled.setCursor(70, 63);
  oled.print("# Confirm");
  oled.sendBuffer();
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
        if (_inputIndex>0) {
          int number=atoi(_inputBuffer);
          _inputIndex=0;
          memset(_inputBuffer, 0, sizeof(_inputBuffer));
          _inputKeyColumn=0;
          if (number < 1 || number > 10239) {
            oled.setCursor(0, 26);
            oled.print("#####");
            oled.setCursor(0, 35);
            oled.print("INVALID ADDRESS");
            oled.sendBuffer();
          } else {
            bool inUse=false;
            for (int i=0; i<NUM_THROTTLES; i++) {
              if (_menuSystem->getThrottles()[i]->addressInUse(number)) inUse=true;
            }
            if (inUse) {
              oled.setCursor(0, 26);
              oled.print("#####");
              oled.setCursor(0, 35);
              oled.print("ADDRESS IN USE");
              oled.sendBuffer();
            } else {
              int throttle=_menuSystem->getCurrentThrottle();
              _menuSystem->getThrottles()[throttle]->setLocoAddress(number, LocoSourceEntry);
              _menuSystem->goHome();
            }
          }
        }
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
        if (_inputIndex<5) {
          _inputBuffer[_inputIndex++]=key;
          oled.setCursor(_inputKeyColumn, 26);
          oled.print(" ");
          oled.setCursor(_inputKeyColumn, 26);
          oled.print(key);
          oled.sendBuffer();
          _inputKeyColumn+=5;
        }
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

void Menu::select(OLED& oled) {
  _menuSystem->setCurrentItem(this);
  this->display(oled);
}

void Menu::display(OLED& oled) {
  int startIndex=_currentPage*_itemsPerPage;
  int endIndex=min(startIndex+_itemsPerPage, _itemCount);
  
  oled.clear();
  oled.setFont(MENU_TITLE_FONT);
  oled.setCursor(0, 6);
  oled.print(_label);
  oled.drawHLine(0, 7, 128);
  int i=0;
  int X=0;
  int Y=17;
  oled.setFont(MENU_ITEM_FONT);
  for (int index=startIndex; index<endIndex; index++) {
    MenuItemBase* item=getItemAtIndex(index);
    oled.setCursor(X, Y);
    oled.print(i);
    oled.setCursor(X+8, Y);
    char* label=item->getLabel();
    if (strlen(label)>10 && _itemCount>5) {
      label[10]='\0';
    }
    oled.print(item->getLabel());
    Y+=8;
    i++;
    if (i==5) {
      X=64;
      Y=17;
    }
  }
  oled.drawHLine(0, 54, 128);
  oled.setCursor(0, 63);
  oled.print("* Back");
  if (_itemCount>_itemsPerPage) {
    oled.setCursor(70, 63);
    oled.print("# Page ");
    int nextPage=(_currentPage+1)%((int)ceil(_itemCount/(float)_itemsPerPage))+1;
    oled.print(nextPage);
  }
  oled.sendBuffer();
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
          item->select(oled);
        }
        break;
      }

      case '*':
        if (this->_parent) {
          _menuSystem->setCurrentItem(this->_parent);
          _parent->select(oled);
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

MenuItemBase* Menu::getItemList() {
  return _itemList;
}

ThrottleMenu::ThrottleMenu(const char* label, int throttleNumber)
  : Menu(label) {
  _throttleNumber=throttleNumber;
}

void ThrottleMenu::select(OLED& oled) {
  _menuSystem->setCurrentItem(this);
  _menuSystem->setCurrentThrottle(_throttleNumber);
  this->display(oled);
}

// class ThrottleScreen
ThrottleScreen::ThrottleScreen()
  : MenuItemBase(nullptr, MenuItemBase::Throttle) {}

void ThrottleScreen::select(OLED& oled) {
  _menuSystem->goHome();
}

void ThrottleScreen::handleKeys(char key, KeyState keyState, OLED& oled) {
  if (_menu) {
    if (keyState==PRESSED) {
      switch(key) {
        case '*':
          _menuSystem->setCurrentItem(_menu);
          _menu->display(oled);
          break;

        case'1': {
          auto th=_menuSystem->getThrottles()[0];
          int speed=th->getSpeed();
          int address=th->getLocoAddress();
          if (speed==0 && address!=0) {
            th->setDirection(!th->getDirection());
            th->displayDirection();
          }
          break;
        }

        if (NUM_THROTTLES>1) {
          case '2': { // Reverse key
            auto th=_menuSystem->getThrottles()[1];
            int speed=th->getSpeed();
            int address=th->getLocoAddress();
            if (speed==0 && address!=0) {
              th->setDirection(!th->getDirection());
              th->displayDirection();
            }
            break;
          }

          case '5': // Light (press)/horn (hold) key
            break;

          case '8': // Display functions key
            break;
        }
        
        if (NUM_THROTTLES>2) {
          case '3': {
            auto th=_menuSystem->getThrottles()[2];
            int speed=th->getSpeed();
            int address=th->getLocoAddress();
            if (speed==0 && address!=0) {
              th->setDirection(!th->getDirection());
              th->displayDirection();
            }
            break;
          }
        }
        
        default:
          break;
      }
    }
  }
}

void ThrottleScreen::setMenu(MenuItemBase* menu) {
  _menu=menu;
  _menu->setParent(this);
}

MenuItemBase* ThrottleScreen::getMenu() {
  return _menu;
}

// class MenuSystem
MenuSystem::MenuSystem(OLED& oled)
  : _oled(oled) {
    _currentItem=nullptr;
    _home=nullptr;
    _currentActionItem=nullptr;
    _throttles=nullptr;
    _currentThrottle=-1;
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
  _oled.clear();
  for (int i=0; i<NUM_THROTTLES; i++) {
    _throttles[i]->displaySpeed(true);
    _throttles[i]->displayDirection();
    _throttles[i]->displayAddress();
  }
  _oled.drawHLine(0, 55, 128);
  _oled.setFont(MENU_ITEM_FONT);
  _oled.setCursor(0, 63);
  _oled.print("* Menu");
  _oled.setCursor(60, 63);
  _oled.print("Trk Power: ");
  _displayPowerState();
  _oled.sendBuffer();
}

void MenuSystem::setCurrentItem(MenuItemBase* currentItem) {
  _currentItem=currentItem;
}

Menu* MenuSystem::findMenuByLabel(const char* label) {
  ThrottleScreen* home=static_cast<ThrottleScreen*>(_home);
  return _findMenuByLabelRecursive(label, home->getMenu());
}

void MenuSystem::setCurrentActionItem(ActionMenuItem *item) {
  _currentActionItem=item;
}

ActionMenuItem* MenuSystem::getCurrentActionItem() {
  return _currentActionItem;
}

void MenuSystem::setThrottles(Throttle** throttles) {
  _throttles=throttles;
}

Throttle** MenuSystem::getThrottles() {
  return _throttles;
}

void MenuSystem::setCurrentThrottle(int throttle) {
  _currentThrottle=throttle;
}

int MenuSystem::getCurrentThrottle() {
  return _currentThrottle;
}

bool MenuSystem::isHome() {
  if (_currentItem==_home) return true;
  return false;
}

void MenuSystem::updatePowerState(TrackPower state) {
  _powerState=state;
  _displayPowerState();
}

Menu* MenuSystem::_findMenuByLabelRecursive(const char* label, MenuItemBase* currentMenuItem) {
  if (!currentMenuItem) {
    return nullptr; // No items matched label
  }
  if (currentMenuItem->getType()==MenuItemBase::Menu) {
    Menu* currentMenu=static_cast<Menu*>(currentMenuItem);
    if (strcmp(currentMenuItem->getLabel(), label)==0) {
      return currentMenu;
    }
    MenuItemBase* item=currentMenu->getItemList();
    while (item) {
      Menu* foundMenu=_findMenuByLabelRecursive(label, item);
      if (foundMenu) {
        return foundMenu;
      }
      item=item->getNext();
    }
  }
  return _findMenuByLabelRecursive(label, currentMenuItem->getNext());
}

void MenuSystem::_displayPowerState() {
  _oled.setFont(MENU_ITEM_FONT);
  _oled.setCursor(113, 63);
  _oled.print("   ");
  _oled.setCursor(113, 63);
  if (_powerState==PowerOn) {
    _oled.print("On");
  } else if (_powerState==PowerOff) {
    _oled.print("Off");
  } else {
    _oled.print("?");
  }
  _oled.sendBuffer();
}
