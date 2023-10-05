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

#include "DisplayFunctions.h"

/***********************************************************************************
Set up OLED libraries and object
***********************************************************************************/
#ifdef USE_OLED_SPI
SSD1306AsciiSpi oled;
#endif
#ifdef USE_OLED_I2C
SSD1306AsciiAvrI2c oled;
#endif

uint8_t loco1Speed = 0;
uint8_t loco2Speed = 0;
uint8_t loco3Speed = 0;
uint8_t newLoco1Speed = 0;
uint8_t newLoco2Speed = 0;
uint8_t newLoco3Speed = 0;
bool loco1Direction = 1;
bool loco2Direction = 0;
bool loco3Direction = 1;
int loco1Address = 0;
int loco2Address = 0;
int loco3Address = 0;

void displayStartupInfo() {
  // Set up serial and display basic config
  Serial.print(F("DCC-EX Serial Throttle "));
  Serial.println(VERSION);
  // Serial.print(F("Averaging inputs over "));
  // Serial.print(SAMPLES);
  // Serial.println(F(" samples"));
  // Set up OLED
#ifdef USE_OLED_SPI
  oled.begin(OLED_TYPE, CS_PIN, DC_PIN);
#endif
#ifdef USE_OLED_I2C
  oled.begin(OLED_TYPE, OLED_ADDRESS);
#endif
  oled.setFont(OLED_FONT);
  oled.clear();
  oled.setCursor(0, 0);
  oled.set1X();
  oled.print("DCC-EX");
  oled.setCursor(0, 2);
  oled.print("Serial Throttle");
  oled.setCursor(0, 4);
  oled.print(VERSION);
}

/*
Display speeds
*/
// void displaySpeeds() {
//   oled.clear();
//   oled.setCursor(4, 0);
//   oled.set2X();
//   oled.print(loco1Speed);
//   oled.clearToEOL();
//   oled.setCursor(46, 0);
//   oled.print(loco2Speed);
//   oled.clearToEOL();
//   oled.setCursor(88, 0);
//   oled.print(loco3Speed);
//   oled.clearToEOL();
//   oled.set1X();
//   oled.setCursor(10, 3);
//   if (loco1Direction) {
//     oled.print(F("Fwd"));
//   } else {
//     oled.print(F("Rev"));
//   }
//   oled.clearToEOL();
//   oled.setCursor(52, 3);
//   if (loco2Direction) {
//     oled.print(F("Fwd"));
//   } else {
//     oled.print(F("Rev"));
//   }
//   oled.clearToEOL();
//   oled.setCursor(94, 3);
//   if (loco3Direction) {
//     oled.print(F("Fwd"));
//   } else {
//     oled.print(F("Rev"));
//   }
// }

void updateLocoSpeed(int startCol, int endCol, int textCol, int speed) {
  oled.set2X();
  oled.clear(startCol, endCol, 0, 2);
  oled.setCursor(textCol, 0);
  oled.print(speed);
}

void updateLocoDirection(int startCol, int endCol, int textCol, bool direction) {
  oled.set1X();
  oled.clear(startCol, endCol, 3, 4);
  oled.setCursor(textCol, 3);
  if (direction) {
    oled.print(F("Rev"));
  } else {
    oled.print(F("Fwd"));
  }
}

void updateLocoAddress(int startCol, int endCol, int textCol, int address) {
  oled.set1X();
  oled.clear(startCol, endCol, 5, 5);
  oled.setCursor(textCol, 5);
  oled.print(address);
}

void updateLoco1Speed() {
  updateLocoSpeed(0, 45, 4, loco1Speed);
}

void updateLoco2Speed() {
  updateLocoSpeed(46, 87, 46, loco2Speed);
}

void updateLoco3Speed() {
  updateLocoSpeed(88, 128, 88, loco3Speed);
}

void updateLoco1Direction() {
  updateLocoDirection(0, 51, 10, loco1Direction);
}

void updateLoco2Direction() {
  updateLocoDirection(52, 93, 52, loco2Direction);
}

void updateLoco3Direction() {
  updateLocoDirection(94, 128, 94, loco3Direction);
}

void updateLoco1Address() {
  updateLocoAddress(0, 46, 5, loco1Address);
}

void updateLoco2Address() {
  updateLocoAddress(47, 88, 47, loco2Address);
}

void updateLoco3Address() {
  updateLocoAddress(89, 128, 89, loco3Address);
}

void displayHomeScreen() {
  oled.clear();
  updateLoco1Speed();
  updateLoco2Speed();
  updateLoco3Speed();
  updateLoco1Direction();
  updateLoco2Direction();
  updateLoco3Direction();
  updateLoco1Address();
  updateLoco2Address();
  updateLoco3Address();
  oled.setCursor(0, 7);
  oled.print("* Menu");
} 

void displayConnectionError() {
  oled.clear();
  oled.setCursor(0, 0);
  oled.set1X();
  oled.print("Connection error");
}

/*
Display directions
*/

/*
Display locos
*/
// void displayLocos() {
//   oled.setCursor(5, 5);
//   oled.print(loco1Address);
//   oled.clearToEOL();
//   oled.setCursor(47, 5);
//   oled.print(loco2Address);
//   oled.clearToEOL();
//   oled.setCursor(89, 5);
//   oled.print(loco3Address);
//   oled.clearToEOL();
// }

/*
Display EStop status
*/
// void displayEStop() {
//   oled.setCursor(0, 0);
//   oled.clearToEOL();
//   oled.setCursor(4, 1);
//   oled.set1X();
//   oled.print("ESTOP");
//   oled.clearToEOL();
//   oled.setCursor(46, 1);
//   oled.print("ESTOP");
//   oled.clearToEOL();
//   oled.setCursor(88, 1);
//   oled.print("ESTOP");
//   oled.clearToEOL();
// }
