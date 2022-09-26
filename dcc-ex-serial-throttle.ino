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
#include "Keypad.h"

// If we haven't got a custom config.h, use the example
#if __has_include ( "config.h")
  #include "config.h"
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif

// Set up OLED, either SPI or I2C
#ifdef USE_OLED_SPI
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiSpi.h"
SSD1306AsciiSpi oled;
#endif
#ifdef USE_OLED_I2C
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
SSD1306Ascii oled;
#endif

// Set up our keypad with key map and instantiate it
char keys[4][3] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[4]      = {KEYPAD_PIN2, KEYPAD_PIN7, KEYPAD_PIN6, KEYPAD_PIN4};
byte pin_column[3] = {KEYPAD_PIN3, KEYPAD_PIN1, KEYPAD_PIN5};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 3);

// Set up our averaging inputs for the potentiometers
AnalogueAverage pot1(POT1_PIN);
AnalogueAverage pot2(POT2_PIN);
AnalogueAverage pot3(POT3_PIN);

// Set global variables with initial values
int8_t loco1Speed = 0;
int8_t loco2Speed = 0;
int8_t loco3Speed = 0;
bool loco1Direction = 1;
bool loco2Direction = 1;
bool loco3Direction = 1;
bool loco1Light = 0;
bool loco2Light = 0;
bool loco3Light = 0;
bool eStop = false;

// Placeholder of static loco addresses until roster functions added
uint16_t loco1Address = 2004;
uint16_t loco2Address = 2006;
uint16_t loco3Address = 2010;

void setup() {
  // Set up serial and display basic config
  Serial.begin(115200);
  Serial.print(F("DCC-EX Serial Throttle "));
  Serial.println(VERSION);
  Serial.print(F("Averaging inputs over "));
  Serial.print(SAMPLES);
  Serial.println(F(" samples"));
  // Set up OLED
#ifdef USE_OLED_SPI
  oled.begin(&SH1106_128x64, CS_PIN, DC_PIN);
#endif
#ifdef USE_OLED_I2C
  oled.begin(&Adafruit128x64, OLED_ADDRESS);
#endif
  oled.setFont(OLED_FONT);
  oled.clear();
  // Set initial display
  displaySpeeds();
  displayLocos();
  // Configure keypad
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(KEYPAD_DEBOUNCE);
  keypad.setHoldTime(KEYPAD_HOLD);
}

void loop() {
  getSerialInput();
  keypad.getKey();
  processSliders();
}

/*
Function to receive any serial input and process it.

All commands start with <, end with >
< should reset and start again

This needs to respond to anything global (eg. eStop, turnout changes) and
anything related to a selected loco

Might be simpler to have two separate functions/routines here:

getBroadcasts() {}
getLoco(locoAddress) {}

This function should probably just get anything between <> and pass it on

The actual parser should probably just look for the first character and discard
anything that's not either a broadcast, or doesn't have the possibility of being
locally relevant

Should it flag for changes to locos from other throttles? Pots will always override
digital throttles, maybe * in front of speed/dir?
*/
void getSerialInput() {

}

/*
Function for a keypad event handler
*/
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      keyPressed(key);
      break;
    case RELEASED:
      break;
    case HOLD:
      keyHeld(key);
      break;
    case IDLE:
      break;
    default:
      break;
  }
}

/*
Function to process individual key presses
*/
void keyPressed(char key) {
  switch(key) {
    case '1':
      if (loco1Speed == 0) {
        loco1Direction = !loco1Direction;
        setLocoThrottle(loco1Address, loco1Speed, loco1Direction);
        displaySpeeds();
      }
      break;
    case '2':
      if (loco2Speed == 0) {
        loco2Direction = !loco2Direction;
        setLocoThrottle(loco2Address, loco2Speed, loco2Direction);
        displaySpeeds();
      }
      break;
    case '3':
      if (loco3Speed == 0) {
        loco3Direction = !loco3Direction;
        setLocoThrottle(loco3Address, loco3Speed, loco3Direction);
        displaySpeeds();
      }
      break;
    case '4':
      loco1Light = !loco1Light;
      setLocoFunction(loco1Address, 0, loco1Light);
      break;
    case '5':
      loco2Light = !loco2Light;
      setLocoFunction(loco2Address, 0, loco2Light);
      break;
    case '6':
      loco3Light = !loco3Light;
      setLocoFunction(loco3Address, 0, loco3Light);
      break;
    default:
      break;
  }
}

/*
Function to process individual key presses
*/
void keyHeld(char key) {
  switch(key) {
    case '0':
      setEstopAll();
      eStop = true;
      break;
    default:
      break;
  }
}

/*
Function to process the potentiometer inputs to control speed
This is a HORRENDOUS function and needs to be optimised
*/
void processSliders() {
  pot1.averageInput();
  pot2.averageInput();
  pot3.averageInput();
  int8_t newL1Speed = map(pot1.getAverage(), POT_MIN, POT_MAX, 0, 126);
  int8_t newL2Speed = map(pot2.getAverage(), POT_MIN, POT_MAX, 0, 126);
  int8_t newL3Speed = map(pot3.getAverage(), POT_MIN, POT_MAX, 0, 126);
  if (eStop == true) {
    displayEStop();
    if (newL1Speed == 0 && newL2Speed == 0 && newL3Speed == 0) {
      eStop = false;
    }
  } else {
    bool updateDisplay = false;
    if (newL1Speed != loco1Speed) {
      loco1Speed = newL1Speed;
      updateDisplay = true;
      setLocoThrottle(loco1Address, loco1Speed, loco1Direction);
    }
    if (newL2Speed != loco2Speed) {
      loco2Speed = newL2Speed;
      updateDisplay = true;
      setLocoThrottle(loco2Address, loco2Speed, loco2Direction);
    }
    if (newL3Speed != loco3Speed) {
      loco3Speed = newL3Speed;
      updateDisplay = true;
      setLocoThrottle(loco3Address, loco3Speed, loco3Direction);
    }
    if (updateDisplay) {
  #ifdef DEBUG
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
  #endif
      displaySpeeds();
      displayLocos();
    }
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

void displayEStop() {
  oled.setCursor(0, 0);
  oled.clearToEOL();
  oled.setCursor(4, 1);
  oled.set1X();
  oled.print("ESTOP");
  oled.clearToEOL();
  oled.setCursor(46, 1);
  oled.print("ESTOP");
  oled.clearToEOL();
  oled.setCursor(88, 1);
  oled.print("ESTOP");
  oled.clearToEOL();
}