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

#include "Throttle.h"
#include "defines.h"
#include "AnalogueAverage.h"







/***********************************************************************************
Set up our pot objects
***********************************************************************************/
AnalogueAverage pot1(POT1_PIN);
AnalogueAverage pot2(POT2_PIN);
AnalogueAverage pot3(POT3_PIN);

int8_t loco1Speed = 0;
int8_t loco2Speed = 0;
int8_t loco3Speed = 0;
bool loco1Direction = 1;
bool loco2Direction = 1;
bool loco3Direction = 1;
bool loco1Light = 0;
bool loco2Light = 0;
bool loco3Light = 0;
bool loco1Stop = false;                   // Flag to temporarily stop loco when button held
bool loco2Stop = false;
bool loco3Stop = false;
bool eStop = false;                       // Flag when 0 held for EStop
bool trackPower = 0;                      // Flag for track power
uint16_t loco1Address = 2010;
uint16_t loco2Address = 2004;
uint16_t loco3Address = 2006;



/***********************************************************************************
Potentiometer processing functions
***********************************************************************************/
/*
Function to process the potentiometer inputs to control speed
This is a HORRENDOUS function and needs to be optimised
*/
// void processSliders() {
//   pot1.averageInput();
//   pot2.averageInput();
//   pot3.averageInput();
//   int8_t newL1Speed;
//   int8_t newL2Speed;
//   int8_t newL3Speed;
//   if (loco1Stop == true) {
//     newL1Speed = 0;
//   } else {
//     newL1Speed = map(pot1.getAverage(), POT_MIN, POT_MAX, 0, 126);
//   };
//   if (loco2Stop == true) {
//     newL2Speed = 0;
//   } else {
//     newL2Speed = map(pot2.getAverage(), POT_MIN, POT_MAX, 0, 126);
//   };
//   if (loco3Stop == true) {
//     newL3Speed = 0;
//   } else {
//     newL3Speed = map(pot3.getAverage(), POT_MIN, POT_MAX, 0, 126);
//   };
//   if (eStop == true) {
//     displayEStop();
//     if (newL1Speed == 0 && newL2Speed == 0 && newL3Speed == 0) {
//       eStop = false;
//     }
//   } else {
//     bool updateDisplay = false;
//     if (newL1Speed != loco1Speed) {
//       loco1Speed = newL1Speed;
//       updateDisplay = true;
//       setLocoThrottle(loco1Address, loco1Speed, loco1Direction);
//     }
//     if (newL2Speed != loco2Speed) {
//       loco2Speed = newL2Speed;
//       updateDisplay = true;
//       setLocoThrottle(loco2Address, loco2Speed, loco2Direction);
//     }
//     if (newL3Speed != loco3Speed) {
//       loco3Speed = newL3Speed;
//       updateDisplay = true;
//       setLocoThrottle(loco3Address, loco3Speed, loco3Direction);
//     }
//     if (updateDisplay) {
//       displaySpeeds();
//       displayLocos();
//     }
//   }
// }
