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
#include "libs/avdweb_Switch.h"
#include "version.h"
#include "AnalogueAverage.h"
#include "libs/avdweb_Switch.h"

// If we haven't got a custom config.h, use the example
#if __has_include ( "config.h")
  #include "config.h"
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif

// If using OLED, include the libraries and instantiate the object
#ifdef USE_OLED_SPI
#include <SPI.h>
#include "libs/SSD1306Ascii.h"
#include "libs/SSD1306AsciiSpi.h"
SSD1306AsciiSpi oled;
#endif
#ifdef USE_OLED_I2C
#include "libs/SSD1306Ascii.h"
#include "libs/SSD1306AsciiAvrI2c.h"
SSD1306Ascii oled;
#endif

AnalogueAverage pot1(A0);
AnalogueAverage pot2(A1);
AnalogueAverage pot3(A2);

int8_t loco1Speed = 0;
int8_t loco2Speed = 0;
int8_t loco3Speed = 0;
bool loco1Direction = 1;
bool loco2Direction = 1;
bool loco3Direction = 1;

uint16_t loco1Address = 2004;
uint16_t loco2Address = 2006;
uint16_t loco3Address = 2010;

void setup() {
  Serial.begin(115200);
  Serial.print(F("DCC-EX Serial Throttle "));
  Serial.println(VERSION);
  Serial.print(F("Averaging inputs over "));
  Serial.print(SAMPLES);
  Serial.println(F(" samples"));
#ifdef USE_OLED_SPI
  oled.begin(&SH1106_128x64, CS_PIN, DC_PIN);
#endif
#ifdef USE_OLED_I2C
  oled.begin(&Adafruit128x64, OLED_ADDRESS);
#endif
  oled.setFont(OLED_FONT);
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
This is a HORRENDOUS function and needs to be optimised.
*/
void processSliders() {
  pot1.averageInput();
  pot2.averageInput();
  pot3.averageInput();
  bool updateDisplay = false;
  uint16_t newPot1 = pot1.getAverage();
  uint16_t newPot2 = pot2.getAverage();
  uint16_t newPot3 = pot3.getAverage();
  int8_t newL1Speed;
  int8_t newL2Speed;
  int8_t newL3Speed;
  bool newL1Direction;
  bool newL2Direction;
  bool newL3Direction;
  if (newPot1 >= FWD_MIN) {
    newL1Speed = map(pot1.getAverage(), FWD_MIN, FWD_MAX, 0, 126);
    newL1Direction = 1;
  } else if (newPot1 <= REV_MIN) {
    newL1Speed = map(pot1.getAverage(), REV_MIN, REV_MAX, 0, 126);
    newL1Direction = 0;
  } else if (newPot1 >= DIR_SWAP) {
    newL1Speed = 0;
    newL1Direction = 1;
  } else if (newPot1 < DIR_SWAP) {
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
  if (newPot3 > 520) {
    newL3Speed = map(pot3.getAverage(), 521, 1020, 0, 126);
    newL3Direction = 1;
  } else if (newPot3 < 500) {
    newL3Speed = map(pot3.getAverage(), 499, 3, 0, 126);
    newL3Direction = 0;
  } else if (newPot3 >= 510) {
    newL3Speed = 0;
    newL3Direction = 1;
  } else if (newPot3 < 510) {
    newL3Direction = 0;
    newL3Speed = 0;
  }
  if (newL1Speed != loco1Speed || newL1Direction != loco1Direction) {
    loco1Speed = newL1Speed;
    loco1Direction = newL1Direction;
    updateDisplay = true;
    setLocoThrottle(loco1Address, loco1Speed, loco1Direction);
  }
  if (newL2Speed != loco2Speed || newL2Direction != loco2Direction) {
    loco2Speed = newL2Speed;
    loco2Direction = newL2Direction;
    updateDisplay = true;
    setLocoThrottle(loco2Address, loco2Speed, loco2Direction);
  }
  if (newL3Speed != loco3Speed || newL3Direction != loco3Direction) {
    loco3Speed = newL3Speed;
    loco3Direction = newL3Direction;
    updateDisplay = true;
    setLocoThrottle(loco3Address, loco3Speed, loco3Direction);
  }
  if (updateDisplay) {
    Serial.print(F("Loco 1/2/3 speed/direction: "));
    Serial.print(loco1Speed);
    Serial.print(F("/"));
    Serial.print(loco1Direction);
    Serial.print(F("/"));
    Serial.print(loco2Speed);
    Serial.print(F("/"));
    Serial.print(loco2Direction);
    Serial.print(F("/"));
    Serial.print(loco3Speed);
    Serial.print(F("/"));
    Serial.println(loco3Direction);
    displaySpeeds();
    displayLocos();
  }
}

/*
OLED Functions below
*/
void displaySpeeds() {
  oled.setCursor(4, 0);
  oled.set2X();
  oled.print(loco1Speed);
  oled.clearToEOL();
  oled.setCursor(46, 0);
  oled.print(loco2Speed);
  oled.clearToEOL();
  oled.setCursor(88, 0);
  oled.print(loco3Speed);
  oled.clearToEOL();
  oled.set1X();
  oled.setCursor(10, 3);
  if (loco1Direction) {
    oled.print(F("Fwd"));
  } else {
    oled.print(F("Rev"));
  }
  oled.clearToEOL();
  oled.setCursor(52, 3);
  if (loco2Direction) {
    oled.print(F("Fwd"));
  } else {
    oled.print(F("Rev"));
  }
  oled.clearToEOL();
  oled.setCursor(94, 3);
  if (loco3Direction) {
    oled.print(F("Fwd"));
  } else {
    oled.print(F("Rev"));
  }
}

void displayLocos() {
  oled.setCursor(5, 5);
  oled.print(loco1Address);
  oled.clearToEOL();
  oled.setCursor(47, 5);
  oled.print(loco2Address);
  oled.clearToEOL();
  oled.setCursor(89, 5);
  oled.print(loco3Address);
  oled.clearToEOL();
}