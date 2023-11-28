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
#include "avdweb_Switch.h"
#include "Rotary.h"

// struct LocoNode {
//   Loco* loco;
//   LocoNode* next;
// };

typedef void (*ThrottleCallback_t)(int throttleNumber);

class Throttle {
public:
  // struct ThrottleAddressList {
  //   LocoNode* addressList;
  // };
  
  /// @brief Constructor
  /// @param throttleNumber 
  /// @param potPin 
  /// @param initialLocoList 
  Throttle(int throttleNumber, /*LocoNode* initialLocoList*/ Loco* loco, int dtPin, int clkPin, int swPin,
            ThrottleCallback_t singleClickCallback,
            ThrottleCallback_t doubleClickCallback,
            ThrottleCallback_t longPressCallback);

  /// @brief Process analogue input for speed
  void process();
  
  /// @brief Get the number of this throttle
  /// @return 
  int getThrottleNumber();
  
  /// @brief Set the loco address and source for this throttle
  /// @param address 
  /// @param source 
  // void setLocoAddress(int address, LocoSource source);
  void setLoco(Loco* loco);
  
  /// @brief Get the loco address for this throttle
  /// @return 
  int getLocoAddress();
  
  /// @brief Check if the throttle speed has changed
  /// @return 
  bool speedChanged();

  void setSpeedChanged();
  
  /// @brief Get the current speed of this throttle
  /// @return 
  int getSpeed();
  
  /// @brief Check if this throttle controls a consist
  /// @return 
  // bool isConsist();
  
  /// @brief Forget the loco associated with this throttle
  /// @param address 
  void forgetLoco(int address);
  
  /// @brief Set the direction for this throttle
  /// @param direction 
  void setDirection(Direction direction);
  
  /// @brief Get the current direction for this throttle
  /// @return 
  Direction getDirection();

  void setDirectionChanged();

  bool directionChanged();
  
  /// @brief Check if the specified address is in use by any throttle
  /// @param address 
  /// @return 
  bool addressInUse(int address);

  static Throttle* findThrottleByLoco(Throttle** throttleArray, int numThrottles, Loco* loco);

private:
  Rotary _encoder;
  Switch _button;
  // int _speed = 0;
  bool _speedChanged=false;
  bool _directionChanged=false;
  // int _locoAddress = 0;
  int _throttleNumber;
  // Direction _direction = Forward;  // Default to forward
  // LocoNode* _locoList = nullptr;  // Linked list containing Locos
  Loco* _loco;
  ThrottleCallback_t _singleClickCallback;
  ThrottleCallback_t _doubleClickCallback;
  ThrottleCallback_t _longPressCallback;

  static void _buttonSingleClickCallback(void* param);
  void _handleSingleClick();
  static void _buttonDoubleClickCallback(void* param);
  void _handleDoubleClick();
  static void _buttonLongPressCallback(void* param);
  void _handleLongPress();

};

#endif
