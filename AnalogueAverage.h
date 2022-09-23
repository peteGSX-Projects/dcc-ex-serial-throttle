/*
 *  © 2022 Peter Cole
 *
 *  This file is for reading a moving average of an analog input.
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

/*
 *  How to use this class
 *
 * This class simply reads an analogue input pin on an Arduino and calculates
 * the moving average across the defined number of samples.
 * 
 * This can be used to calculate the average of multiple analogue inputs by
 * instantiating multiple objects, however all will be averaged across the
 * same number of samples.
 * 
 * Include the class with:
 * 
 * #include "AnalogueAverage.h"
 * 
 * The default number of samples is 20, but this can be overridden at compile
 * time by using:
 * 
 * #define SAMPLES XX.
 * 
 * Instantiate each object with:
 * 
 * AnalogeAverage input(pin)
 * 
 * To ensure continued averaging, you must call the averageInput() function
 * continuously within your main loop() function:
 * 
 * void loop() {
 *   input.averageInput();
 *   ...
 * }
 * 
 * To obtain the current moving object, call the getAverage() function:
 * 
 * uint16_t movingAverage = input.getAverage();
*/

#ifndef ANALOGAVERAGE_H
#define ANALOGAVERAGE_H

#include "Arduino.h"

#ifndef SAMPLES
#define SAMPLES 10
#endif

class AnalogueAverage {
  public:
    AnalogueAverage(uint8_t pin);
    void averageInput();
    uint16_t getAverage();

  private:
    uint8_t _pin;
    uint8_t _currentIndex = 0;
    uint8_t _valueCount = 0;
    uint16_t _sum = 0;
    uint16_t _values[SAMPLES];
    uint16_t _rollingAverage = 0;

};

#endif