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

#ifndef DEFINES_H
#define DEFINES_H

/***********************************************************************************
If we haven't got a custom config.h, use the example
***********************************************************************************/
#if __has_include ( "config.h")
  #include "config.h"
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif

// If samples haven't been defined, use the default of 10
#ifndef SLIDER_SAMPLES
#define SLIDER_SAMPLES 10
#endif

// If retries haven't been defined, use the default of 5
#ifndef CONNECT_RETRIES
#define CONNECT_RETRIES 5
#endif

#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
#undef CONSOLE
#undef CLIENT
#define CONSOLE Serial
#define CLIENT Serial1
#else
#undef CONSOLE
#undef CLIENT
#define CONSOLE Serial
#define CLIENT Serial
#endif

#ifdef USE_OLED_SPI
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiSpi.h"
#define OLED SSD1306AsciiSpi
#endif
#ifdef USE_OLED_I2C
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define OLED SSD1306AsciiAvrI2c
#endif

#endif
