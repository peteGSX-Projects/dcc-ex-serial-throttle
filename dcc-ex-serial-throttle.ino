/*
 *  © 2022 Peter Cole
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

// Include the required libraries
#include <Arduino.h>
#include "dcc-ex-api.h"
#include "avdweb_Switch.h"
#include "version.h"
#include "AnalogueAverage.h"

// If we haven't got a custom config.h, use the example
#if __has_include ( "config.h")
  #include "config.h"
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif

// If using OLED, include the libraries and instantiate the object
#ifdef USE_OLED
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiSpi.h"
SSD1306AsciiSpi oled;
#endif

AnalogueAverage pot1(A0);
AnalogueAverage pot2(A1);

int8_t loco1Speed = 0;
int8_t loco2Speed = 0;
bool loco1Direction = 1;
bool loco2Direction = 1;

void setup() {
  Serial.begin(115200);
  oled.begin(&SH1106_128x64, CS_PIN, DC_PIN);
  oled.setFont(Callibri11);
  oled.clear();
}

void loop() {

  processSliders();
  
}

/*
Throttle input functions here
*/

/*
Function to process the potentiometer inputs to control speed
*/
void processSliders() {
  pot1.averageInput();
  pot2.averageInput();
  bool updateDisplay = false;
  uint16_t newPot1 = pot1.getAverage();
  uint16_t newPot2 = pot2.getAverage();
  int8_t newL1Speed;
  int8_t newL2Speed;
  bool newL1Direction;
  bool newL2Direction;
  if (newPot1 > 520) {
    newL1Speed = map(pot1.getAverage(), 521, 1020, 0, 126);
    newL1Direction = 1;
  } else if (newPot1 < 500) {
    newL1Speed = map(pot1.getAverage(), 499, 3, 0, 126);
    newL1Direction = 0;
  } else if (newPot1 >= 510) {
    newL1Speed = 0;
    newL1Direction = 1;
  } else if (newPot1 < 510) {
    newL1Speed = 0;
    newL1Direction = 0;
  }
  if (newPot2 > 520) {
    newL2Speed = map(pot2.getAverage(), 521, 1020, 0, 126);
    newL2Direction = 1;
  } else if (newPot2 < 500) {
    newL2Speed = map(pot2.getAverage(), 499, 3, 0, 126);
    newL2Direction = 0;
  } else if (newPot2 >= 510) {
    newL2Speed = 0;
    newL2Direction = 1;
  } else if (newPot2 < 510) {
    newL2Direction = 0;
    newL2Speed = 0;
  }
  if (newL1Speed != loco1Speed || newL1Direction != loco1Direction) {
    loco1Speed = newL1Speed;
    loco1Direction = newL1Direction;
    updateDisplay = true;
  }
  if (newL2Speed != loco2Speed || newL2Direction != loco2Direction) {
    loco2Speed = newL2Speed;
    loco2Direction = newL2Direction;
    updateDisplay = true;
  }
  if (updateDisplay) {
    Serial.print(F("Loco 1/2 speed/direction: "));
    Serial.print(loco1Speed);
    Serial.print(F("/"));
    Serial.print(loco1Direction);
    Serial.print(F("/"));
    Serial.print(loco2Speed);
    Serial.print(F("/"));
    Serial.println(loco2Direction);
    displaySpeeds();
  }
}

/*
OLED Functions below
*/
void displaySpeeds() {
  oled.setCursor(10, 0);
  oled.set2X();
  if (loco1Direction) {
    oled.print(F("F"));
  } else {
    oled.print(F("R"));
  }
  oled.print(loco1Speed);
  oled.clearToEOL();
  oled.setCursor(74, 0);
  if (loco2Direction) {
    oled.print(F("F"));
  } else {
    oled.print(F("R"));
  }
  oled.print(loco2Speed);
  oled.clearToEOL();
}
