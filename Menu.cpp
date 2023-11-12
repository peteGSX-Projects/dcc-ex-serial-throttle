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

// #include <Arduino.h>
// #include "Menu.h"

/*
// Create menus
Menu homeScreen("Home Screen", nullptr);
Menu mainMenu("Main Menu", &homeScreen);
Menu setupThrottles("Setup Throttles", &mainMenu);
Menu rosterList("Roster List", &mainMenu);
Menu routeList("Route List", &mainMenu);
Menu turnoutList("Turnout List", &mainMenu);
Menu turntableList("Turntable List", &mainMenu);
Menu trackManagement("Track Management", &mainMenu);
Menu manageLocoConsist("Manage Loco/Consist", &setupThrottles);
Menu inputLocoAddress("Input Loco Address", &manageLocoConsist);
Menu trackManager("TrackManager", &trackManagement);

Menu* currentMenuPtr = &homeScreen;

Throttle* currentThrottle = nullptr;

uint8_t inputKeyColumn = 0;

// Public functions
void Menu::addMenu(int index, const char* label, void *objectPointer) {
  MenuItem* newItem = new MenuItem(index, label, MENU, objectPointer, nullptr);
  _addItem(newItem);
}

void Menu::addActionItem(int index, const char* label, void *objectPointer, void (*callback)()) {
  MenuItem* newItem = new MenuItem(index, label, ACTION, objectPointer, callback);
  _addItem(newItem);
}

void Menu::addEntryItem(int index, const char* label, void (*callback)()) {
  MenuItem* newItem = new MenuItem(index, label, ENTRY, nullptr, callback);
  _addItem(newItem);
}

void Menu::setParent(Menu* parent) {
  _parentMenu = parent;
}

void Menu::display() {
  currentMenuPtr = this;
  _displayMenu();
}

void Menu::handleKeyPress(char key, KeyState keyState){
  if (_inputMode) {
    if (key >= '0' && key <= '9') {
      if (_inputIndex < 5) {
        _inputBuffer[_inputIndex++] = key;
        oled.setCursor(inputKeyColumn, 3);
        oled.print(key);
        inputKeyColumn+= 6;
      }
    } else if (key == '#') {
      if (_inputIndex > 0) {
        int number = atoi(_inputBuffer);
        _inputIndex = 0;
        memset(_inputBuffer, 0, sizeof(_inputBuffer));
        inputKeyColumn = 0;
        if (number < 1 || number > 10239) {
          oled.setCursor(0, 3);
          oled.print("#####");
          oled.setCursor(0, 6);
          oled.print("INVALID ADDRESS");
        } else if (throttle1.addressInUse(number) || throttle2.addressInUse(number) || throttle3.addressInUse(number)) {
          oled.setCursor(0, 3);
          oled.print("#####");
          oled.setCursor(0, 6);
          oled.print("ADDRESS IN USE");
        } else {
          _inputMode = false;
          _isLocoInput = false;
          currentThrottle->setLocoAddress(number, LocoSourceEntry);
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
        if (currentMenuPtr == &homeScreen) {
          _doHomeFunctions(key);
        } else {
          if (getItemCount() > 0) {
            int index = key - '0' + (_currentPage - 1) * 9;
            if (index < getItemCount()) {
              MenuItem item = getItem(index);
              _selectedItemIndex = item.index;
              switch (item.menuItemType) {
                case MENU: {
                  Menu* menu = static_cast<Menu*>(item.objectPointer);
                  menu->display();
                  break;
                }

                case ACTION: {
                  item.callback();
                  break;
                }

                case ENTRY: {
                  break;
                }

                default:
                  break;
              }
            }
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
  return MenuItem{0, "", MENU, nullptr, nullptr};
}

int Menu::getSelectedItem() {
  return _selectedItemIndex;
}

void Menu::setInputMode() {
  _inputMode = true;
}

// Private functions
void Menu::_addItem(MenuItem* newItem) {
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

// Display the menu with a single page of options
void Menu::_displayMenu(){
  // if (_parentMenu == nullptr) {
  //   displayHomeScreen();
  // } else {
  //   oled.clear();
  //   oled.set1X();
  //   oled.setCursor(0, 0);
  //   oled.print(_label);

  //   int startIdx = (_currentPage - 1) * 10;
  //   int endIdx = min(startIdx + 10, getItemCount());

  //   // Display number keys 0 -> 9 to select
  //   int key = 0;
  //   int column = 0;
  //   int row = 1;
  //   for (int i = startIdx; i < endIdx; i++) {
  //     oled.setCursor(column, row);
  //     oled.print(key);
  //     oled.print(" ");
  //     oled.print(getItem(i).label);
  //     key++;
  //     // If next key would be 10, make it 0
  //     if (key > 9) {
  //       key = 0;
  //     }
  //     row++;
  //     // 6th row means row 1 in second column
  //     if (row > 5) {
  //       row = 1;
  //       column = 65;
  //     }
  //   }

  //   oled.setCursor(0, 7);
  //   oled.print("* Back");
  //   if (getItemCount() > 10) {
  //     oled.setCursor(65, 7);
  //     oled.print("# Next page");
  //   }
  // }

  // The top most menu only displays the * key to access the menu system
  if (_parentMenu == nullptr) {
    displayHomeScreen();
  } else if (_inputMode) {
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
    oled.setCursor(65, 7);
    oled.print("# Confirm");
  } else {
  // All other menus show 10 per page in two columns
    oled.clear();
    oled.set1X();
    oled.setCursor(0, 0);
    oled.print(_label);

    if (getItemCount() > 0) {
    
      int startIdx = (_currentPage - 1) * 10;
      int endIdx = min(startIdx + 10, getItemCount());

      // Display number keys 1 -> 0 (instead of 10) to select
      int key = 0;
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
          column = 65;
        }
      }
    }

    oled.setCursor(0, 7);
    oled.print("* Back");
    if (getItemCount() > 10) {
      oled.setCursor(65, 7);
      oled.print("# Next page");
    }
  }
}

// Process keys on the home screen
void Menu::_doHomeFunctions(char key) {
  switch (key) {
    case '1':
      if (throttle1.getSpeed() == 0 && throttle1.getLocoAddress() != 0) {
        throttle1.setDirection(!throttle1.getDirection());
        displayThrottle1Direction();
      }
      break;
    case '2':
      if (throttle2.getSpeed() == 0 && throttle2.getLocoAddress() != 0) {
        throttle2.setDirection(!throttle2.getDirection());
        displayThrottle2Direction();
      }
      break;
    case '3':
      if (throttle3.getSpeed() == 0 && throttle3.getLocoAddress() != 0) {
        throttle3.setDirection(!throttle3.getDirection());
        displayThrottle3Direction();
      }
      break;
    default:
      break;
  }
}

// End of class

// Function to create required menu structure including static list items
void createStaticMenus() {
  // Create menu structure
  // mainMenu.setParent(&homeScreen);
  // setupThrottles.setParent(&mainMenu);
  // rosterList.setParent(&mainMenu);
  // routeList.setParent(&mainMenu);
  // turnoutList.setParent(&mainMenu);
  // turntableList.setParent(&mainMenu);
  // trackManagement.setParent(&mainMenu);
  // manageLocoConsist.setParent(&setupThrottles);
  // inputLocoAddress.setParent(&manageLocoConsist);
  // trackManager.setParent(&trackManagement);

  // Setup main menu
  mainMenu.addMenu(0, "Throttles", &setupThrottles);
  mainMenu.addMenu(1, "Turnouts", &turnoutList);
  mainMenu.addMenu(2, "Routes", &routeList);
  mainMenu.addMenu(3, "Turntables", &turntableList);
  mainMenu.addMenu(4, "Roster", &rosterList);
  mainMenu.addMenu(5, "Tracks", &trackManagement);

  // Setup throttle menu
  setupThrottles.addActionItem(0, "Throttle 1", nullptr, setThrottleContext);
  setupThrottles.addActionItem(1, "Throttle 2", nullptr, setThrottleContext);
  setupThrottles.addActionItem(2, "Throttle 3", nullptr, setThrottleContext);

  // Setup manage throttle menu
  manageLocoConsist.addActionItem(0, "Select from roster", nullptr, selectFromRoster);
  manageLocoConsist.addActionItem(1, "Enter address", nullptr, enterLocoAddress);
  manageLocoConsist.addActionItem(2, "Remove loco", nullptr, forgetLoco);
  manageLocoConsist.addActionItem(3, "Display consist", nullptr, noAction);
  manageLocoConsist.addActionItem(4, "Forget loco/consist", nullptr, noAction);

  // Setup track management
  trackManagement.addActionItem(0, "Power On", nullptr, setTrackPower);
  trackManagement.addActionItem(1, "Power Off", nullptr, setTrackPower);
  trackManagement.addActionItem(2, "Join", nullptr, setJoinTracks);
  trackManagement.addActionItem(3, "Unjoin", nullptr, setJoinTracks);
  trackManagement.addMenu(4, "TrackManager", &trackManager);

  // Setup TrackManager
  trackManager.addActionItem(0, "Track A", nullptr, noAction);
  trackManager.addActionItem(1, "Track B", nullptr, noAction);
  trackManager.addActionItem(2, "Track C", nullptr, noAction);
  trackManager.addActionItem(3, "Track D", nullptr, noAction);
  trackManager.addActionItem(4, "Track E", nullptr, noAction);
  trackManager.addActionItem(5, "Track F", nullptr, noAction);
  trackManager.addActionItem(6, "Track G", nullptr, noAction);
  trackManager.addActionItem(7, "Track H", nullptr, noAction);
  trackManager.addActionItem(8, "Show Tracks", nullptr, noAction);
  trackManager.addActionItem(9, "Dummy 1", nullptr, noAction);
  trackManager.addActionItem(10, "Dummy 2", nullptr, noAction);
  trackManager.addActionItem(11, "Dummy 3", nullptr, noAction);

}

// Function to display the correct context when managing a throttle
// - Displays the same menu options regardless of selected throttle
// - Updates the parent to keep it in the right menu context
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
  manageLocoConsist.display();
  currentMenuPtr->setParent(tempPtr);
}

// Helper function to get input from the keypad
void enterLocoAddress() {
  inputLocoAddress.setInputMode();
  inputLocoAddress.display();
}

// Helper function to forget loco
void forgetLoco() {
  if (currentThrottle->isConsist()) return;
  currentThrottle->forgetLoco(currentThrottle->getLocoAddress());
  homeScreen.display();
}

// Helper function to show the roster list when selecting a loco
void selectFromRoster() {
  rosterList.setParent(currentMenuPtr);
  rosterList.display();
}

// Helper function to set the loco address from the roster list
// and update the parent correctly
void setLocoFromRoster() {
  // if (rosterList.getParent() != &mainMenu) {
  //   uint16_t address = currentMenuPtr->getItem(currentMenuPtr->getSelectedItem()).objectId;
  //   if (throttle1.addressInUse(address) || throttle2.addressInUse(address) || throttle3.addressInUse(address)) {
  //     oled.setCursor(0, 6);
  //     oled.print("ADDRESS IN USE");
  //   } else {
  //     if (dccexProtocol.throttleConsists[currentThrottle->getThrottleNumber()].consistGetNumberOfLocos() == 0) {
  //       currentThrottle->setLocoAddress(address, LocoSourceRoster);
  //       rosterList.setParent(&mainMenu);
  //       homeScreen.display();rosterList.setParent(&mainMenu);
  //       homeScreen.display();
  //     } else {
  //       // This needs to add another loco but needs to figure out facing forward or reverse.
  //     }
  //   }
  // }
}

// Function for menu items that just display info
void noAction() {}
*/