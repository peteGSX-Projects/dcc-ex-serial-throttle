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

#ifndef THROTTLE_H
#define THROTTLE_H

#include <Arduino.h>
#include "defines.h"
#include "DCCEXObjects.h"
#include "DisplayFunctions.h"

struct LocoNode {
  Loco* loco;
  LocoNode* next;
};

class Throttle {
public:
  struct ThrottleAddressList {
    LocoNode* addressList;
  };
  
  /// @brief Constructor
  /// @param throttleNumber 
  /// @param potPin 
  /// @param initialLocoList 
  Throttle(int throttleNumber, int potPin, LocoNode* initialLocoList, OLED& oled);

  /// @brief Process analogue input for speed
  void process();
  
  /// @brief Get the number of this throttle
  /// @return 
  int getThrottleNumber();
  
  /// @brief Set the loco address and source for this throttle
  /// @param address 
  /// @param source 
  void setLocoAddress(int address, LocoSource source);
  
  /// @brief Get the loco address for this throttle
  /// @return 
  int getLocoAddress();
  
  /// @brief Check if the throttle speed has changed
  /// @return 
  bool speedChanged();
  
  /// @brief Get the current speed of this throttle
  /// @return 
  int getSpeed();
  
  /// @brief Check if this throttle controls a consist
  /// @return 
  bool isConsist();
  
  /// @brief Forget the loco associated with this throttle
  /// @param address 
  void forgetLoco(int address);
  
  /// @brief Set the direction for this throttle
  /// @param direction 
  void setDirection(bool direction);
  
  /// @brief Get the current direction for this throttle
  /// @return 
  bool getDirection();
  
  /// @brief Check if another throttle has overridden the speed/direction
  /// @return 
  bool isOverridden();
  
  /// @brief Check if the specified address is in use by this throttle
  /// @param address 
  /// @return 
  bool addressInUse(int address);

  /// @brief Display the speed on screen for this throttle
  void displaySpeed(bool isHome);

  /// @brief Display the direction on screen for this throttle
  void displayDirection();

  /// @brief Display the address on screen for this throttle
  void displayAddress();

  /// @brief Display EStop for this throttle instead of speed
  void displayEStop();

private:
  int _potPin;  // pin the potentiometer is on for this throttle
  int _currentIndex = 0;
  int _valueCount = 0;
  int _sum = 0;
  int _values[SLIDER_SAMPLES];
  int _speed = 0;
  int _rollingAverage = 0;
  bool _speedChanged = false;
  int _locoAddress = 0;
  int _throttleNumber;
  bool _direction = Forward;  // Default to forward
  bool _isOverridden = false;
  LocoNode* _locoList = nullptr;  // Linked list containing Locos
  OLED& _oled;

  /// @brief Helper function to get the direction name
  /// @param direction 
  /// @return 
  Direction _getDirectionName(bool direction);

};

#endif
