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
#include "Rotary.h"
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
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
SSD1306AsciiAvrI2c oled;
#endif

// Define our pins in use
#define ROTARY_BTN 2
#define ROTARY_DT 5
#define ROTARY_CLK 6

void setup() {
  Serial.begin(115200);
}

void loop() {

}

#ifdef USE_OLED
/*
OLED Functions below here if OLED in use.
*/

// This function updates the OLED with the provided text at the cursor position
void oledUpdate(int column, int row, String textToPrint) {
  char textChar[textToPrint.length() + 1];
  textToPrint.toCharArray(textChar, textToPrint.length() + 1);
  oled.setCursor(column, row);
  oled.clearToEOL();
  oled.write(textChar);
}
#endif
