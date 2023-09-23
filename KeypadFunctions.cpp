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

#include "KeypadFunctions.h"

bool keyPress = false;                    // Flag for when a key is pressed rather than held

/***********************************************************************************
Set up our keypad
***********************************************************************************/
char keys[4][3] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[4] = {KEYPAD_PIN2, KEYPAD_PIN7, KEYPAD_PIN6, KEYPAD_PIN4};
byte pin_column[3] = {KEYPAD_PIN3, KEYPAD_PIN1, KEYPAD_PIN5};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 3);

/*
Function to set up the keypad object
*/
void setupKeypad() {
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(KEYPAD_DEBOUNCE);
  keypad.setHoldTime(KEYPAD_HOLD);
}

/*
Function for a keypad event handler
*/
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      keyPress = true;
      break;
    case HOLD:
      keyPress = false;
      // Hold function here
      break;
    case RELEASED:
      if (keyPress == true) {
        // Press function here
      }
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
// void keyPressed(char key) {
//   switch(key) {
//     case '0':
//       trackPower = !trackPower;
//       setTrackPower(trackPower);
//       break;
//     case '1':
//       if (loco1Address > 0 && loco1Speed == 0) {
//         loco1Direction = !loco1Direction;
//         setLocoThrottle(loco1Address, loco1Speed, loco1Direction);
//         displaySpeeds();
//       }
//       break;
//     case '2':
//       if (loco2Address > 0 && loco2Speed == 0) {
//         loco2Direction = !loco2Direction;
//         setLocoThrottle(loco2Address, loco2Speed, loco2Direction);
//         displaySpeeds();
//       }
//       break;
//     case '3':
//       if (loco3Address > 0 && loco3Speed == 0) {
//         loco3Direction = !loco3Direction;
//         setLocoThrottle(loco3Address, loco3Speed, loco3Direction);
//         displaySpeeds();
//       }
//       break;
//     case '4':
//       if (loco1Address > 0) {
//         loco1Light = !loco1Light;
//         setLocoFunction(loco1Address, 0, loco1Light);
//       }
//       break;
//     case '5':
//       if (loco2Address > 0) {
//         loco2Light = !loco2Light;
//         setLocoFunction(loco2Address, 0, loco2Light);
//       }
//       break;
//     case '6':
//       if (loco3Address > 0) {
//         loco3Light = !loco3Light;
//         setLocoFunction(loco3Address, 0, loco3Light);
//       }
//       break;
//     default:
//       break;
//   }
// }

/*
Function to process held keys
*/
// void keyHeld(char key) {
//   switch(key) {
//     case '0':
//       setEstopAll();
//       eStop = true;
//       break;
//     case '7':
//       if (loco1Address > 0) {
//         loco1Stop = true;
//       }
//       break;
//     case '8':
//       if (loco2Address > 0) {
//         loco2Stop = true;
//       }
//       break;
//     case '9':
//       if (loco3Address > 0) {
//         loco3Stop = true;
//       }
//       break;
//     default:
//       break;
//   }
// }

/*
Function to process held keys when released
*/
// void keyReleased(char key) {
//   switch(key) {
//     case '7':
//       loco1Stop = false;
//       break;
//     case '8':
//       loco2Stop = false;
//       break;
//     case '9':
//       loco3Stop = false;
//       break;
//     default:
//       break;
//   }
// }
