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
#include "Menu.h"

// Create menus
Menu homeScreen("Home Screen");
Menu mainMenu("Main Menu");
Menu setupThrottles("Setup Throttles");
Menu rosterList("Roster List");
Menu routeList("Route List");
Menu turnoutList("Turnout List");
Menu turntableList("Turntable List");
Menu trackManagement("Track Management");
Menu manageThrottle("Manage Throttle");
Menu inputLocoAddress("Input Loco Address");

Menu* currentMenuPtr = &homeScreen;

Throttle* currentThrottle = nullptr;

uint8_t inputKeyColumn = 0;

/*
Public functions
*/
void Menu::addItem(int index, const char* label, int16_t objectId, void (*action)()) {
  char* labelCopy = strdup(label);

  if (!labelCopy) {
    return;
  }
  
  MenuItem* newItem = new MenuItem(index, labelCopy, objectId, action);
  newItem->next = nullptr;

  if (!_head) {
    _head = newItem;
  } else {
    // Find last list item
    MenuItem* current = _head;
    while (current->next != nullptr) {
      current = current->next;
    }
    // Add to end of list
    current->next = newItem;
  }
}

void Menu::setParent(Menu* parent) {
  _parentMenu = parent;
}

void Menu::display() {
  currentMenuPtr = this;
  _displayMenu();
}

void Menu::handleKeyPress(char key){
  if (_inputMode) {
    if (key >= '0' && key <= '9') {
      if (_inputIndex < 5) {
        _inputBuffer[_inputIndex++] = key;
        oled.setCursor(inputKeyColumn, 3);
        oled.print(key);
        inputKeyColumn++;
      }
    } else if (key == '#') {
      if (_inputIndex > 0) {
        int number = atoi(_inputBuffer);
        _inputIndex = 0;
        memset(_inputBuffer, 0, sizeof(_inputBuffer));
        inputKeyColumn = 0;
        if (number < 1 || number > 10239) {
          oled.setCursor(0, 4);
          oled.print("#####");
          oled.setCursor(0, 5);
          oled.print("INVALID ADDRESS");
        } else {
          _inputMode = false;
          _isLocoInput = false;
          currentThrottle->setLocoAddress(number);
          currentMenuPtr = &homeScreen;
          currentMenuPtr->display();
        }
      }
    } else if (key == '*') {
      if (currentMenuPtr->getParent() == nullptr) {
        currentMenuPtr = &mainMenu;
      } else if (currentMenuPtr->getParent() != nullptr) {
        currentMenuPtr = currentMenuPtr->getParent();
      }
      inputKeyColumn = 0;
      _isLocoInput = false;
      currentMenuPtr->display();
    }
  } else {
    switch (key) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        if (getItemCount() > 0) {
          int index = key - '1' + (_currentPage - 1) * 9;
          if (index < getItemCount()) {
            _selectedItemIndex = getItem(index).index;
            getItem(index).action();
          }
        }
        break;
      case '#':
        if (_currentPage < (getItemCount() / 9) + 1) {
          _currentPage++;
        } else {
          _currentPage = 1;
        }
        if (getItemCount() > 10) {
          _displayMenu();
        }
        break;
      case '*':
        if (currentMenuPtr->getParent() == nullptr) {
          currentMenuPtr = &mainMenu;
        } else if (currentMenuPtr->getParent() != nullptr) {
          currentMenuPtr = currentMenuPtr->getParent();
        }
        currentMenuPtr->display();
        break;
      default:
        break;
    }
  }
}

int Menu::getItemCount() {
  int count = 0;
  MenuItem* current = _head;
  
  while (current != nullptr) {
    count++;
    current = current->next;
  }

  return count;
}

Menu* Menu::getParent() {
  return _parentMenu;
}

MenuItem Menu::getItem(int index) {
  int currentIndex = 0;
  MenuItem* currentItem = _head;

  while (currentItem != nullptr) {
    if (currentIndex == index) {
      return *currentItem;
    }
    currentIndex++;
    currentItem = currentItem->next;
  }
  return MenuItem{0, "", 0, nullptr};
}

int Menu::getSelectedItem() {
  return _selectedItemIndex;
}

void Menu::setInputMode() {
  _inputMode = true;
}

/*
Private functions
*/
// Display the menu with a single page of options
void Menu::_displayMenu(){
  // The top most menu only displays the * key to access the menu system
  if (_parentMenu == nullptr) {
    displayHomeScreen();
  } else if (_isLocoInput) {
  // For entering a loco address manually
    oled.clear();
    oled.set1X();
    oled.setCursor(0, 0);
    oled.print("Enter loco address");
    oled.setCursor(0, 1);
    oled.print("for throttle ");
    oled.print(currentThrottle->getThrottleNumber());
    oled.setCursor(0, 3);
    oled.print("#####");
    oled.setCursor(0, 7);
    oled.print("* Back");
    oled.setCursor(60, 7);
    oled.print("# Confirm");
  } else {
  // All other menus show 10 per page in two columns
    oled.clear();
    oled.set1X();
    oled.setCursor(0, 0);
    oled.print(_label);

    int startIdx = (_currentPage - 1) * 10;
    int endIdx = min(startIdx + 10, getItemCount());

    // Display number keys 1 -> 0 (instead of 10) to select
    int key = 1;
    int column = 0;
    int row = 1;
    for (int i = startIdx; i < endIdx; i++) {
      oled.setCursor(column, row);
      oled.print(key);
      oled.print(" ");
      oled.print(getItem(i).label);
      key++;
      // If next key would be 10, make it 0
      if (key > 9) {
        key = 0;
      }
      row++;
      // 6th row means row 1 in second column
      if (row > 5) {
        row = 1;
        column = 60;
      }
    }

    oled.setCursor(0, 7);
    oled.print("* Back");
    if (getItemCount() > 10) {
      oled.setCursor(60, 7);
      oled.print("# Next page");
    }
  }
}

// End of class

/*
Function to create required menu structure including static list items
*/
void createMenus() {
  // Create menu structure
  mainMenu.setParent(&homeScreen);
  setupThrottles.setParent(&mainMenu);
  rosterList.setParent(&mainMenu);
  routeList.setParent(&mainMenu);
  turnoutList.setParent(&mainMenu);
  turntableList.setParent(&mainMenu);
  trackManagement.setParent(&mainMenu);
  manageThrottle.setParent(&setupThrottles);
  inputLocoAddress.setParent(&manageThrottle);

  // Setup main menu
  mainMenu.addItem(0, "Throttles", 0, []() { setupThrottles.display(); });
  mainMenu.addItem(1, "Turnouts", 0, []() { turnoutList.display(); });
  mainMenu.addItem(2, "Routes", 0, []() { routeList.display(); });
  mainMenu.addItem(3, "Turntables", 0, []() { turntableList.display(); });
  mainMenu.addItem(4, "Roster", 0, []() { rosterList.display(); });
  mainMenu.addItem(5, "Tracks", 0, []() { trackManagement.display(); });

  // Setup throttle menu
  setupThrottles.addItem(0, "Throttle 1", 0, setThrottleContext);
  setupThrottles.addItem(1, "Throttle 2", 0, setThrottleContext);
  setupThrottles.addItem(2, "Throttle 3", 0, setThrottleContext);

  // Setup manage throttle menu
  manageThrottle.addItem(0, "Select from roster", 0, dummy);
  manageThrottle.addItem(1, "Enter address", 0, []() { inputLocoAddress.display(); });
  manageThrottle.addItem(2, "Setup consist", 0, dummy);
  manageThrottle.addItem(3, "Forget loco", 0, forgetLoco);

  // Setup track management
  trackManagement.addItem(0, "Power On", 0, dummy);
  trackManagement.addItem(1, "Power Off", 0, dummy);
  trackManagement.addItem(2, "Join", 0, dummy);
  trackManagement.addItem(3, "Unjoin", 0, dummy);
  trackManagement.addItem(4, "TrackManager", 0, dummy);

}

/*
Function to display the correct context when managing a throttle
- Displays the same menu options regardless of selected throttle
- Updates the parent to keep it in the right menu context
*/
void setThrottleContext() {
  if (currentMenuPtr->getSelectedItem() == 0) {
    currentThrottle = &throttle1;
  } else if (currentMenuPtr->getSelectedItem() == 1) {
    currentThrottle = &throttle2;
  } else if (currentMenuPtr->getSelectedItem() == 2) {
    currentThrottle = &throttle3;
  } else {
    currentThrottle = nullptr;
  }
  Menu* tempPtr = currentMenuPtr;
  manageThrottle.display();
  currentMenuPtr->setParent(tempPtr);
}

/*
Function to get input from the keypad
*/
void getInput() {
  currentMenuPtr->setInputMode();
  currentMenuPtr->display();
}

/*
Help function to forget loco
*/
void forgetLoco() {
  currentThrottle->forgetLoco();
  currentMenuPtr = &homeScreen;
  currentMenuPtr->display();
}

/*
Placeholder menu action
*/
void dummy() {
  if (currentMenuPtr != nullptr) {
    int16_t objectId = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
    CONSOLE.print(F("Selected item object ID: "));
    CONSOLE.println(objectId);
  }
}
