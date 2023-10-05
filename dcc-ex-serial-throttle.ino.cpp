# 1 "C:\\Users\\Peter\\AppData\\Local\\Temp\\tmp45sqtw7d"
#include <Arduino.h>
# 1 "C:/Code/dcc-ex-serial-throttle/dcc-ex-serial-throttle.ino"
# 24 "C:/Code/dcc-ex-serial-throttle/dcc-ex-serial-throttle.ino"
#include <Arduino.h>
#include "defines.h"

#include "DisplayFunctions.h"
#include "KeypadFunctions.h"
#include "SerialFunctions.h"
#include "AnalogueAverage.h"
#include "DCCEXObjects.h"
#include "Menu.h"

AnalogueAverage pot1(POT1_PIN);
AnalogueAverage pot2(POT2_PIN);
AnalogueAverage pot3(POT3_PIN);
void setup();
void loop();
void disableJTAG();
#line 42 "C:/Code/dcc-ex-serial-throttle/dcc-ex-serial-throttle.ino"
void setup() {
#if defined(ARDUINO_BLUEPILL_F103C8)
  disableJTAG();
#endif
  pinMode(POT1_PIN, INPUT);

  CONSOLE.begin(115200);
#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
  CLIENT.begin(115200);
#endif
  displayStartupInfo();
  delay(2000);
  setupKeypad();
  dccexProtocol.setLogStream(&CONSOLE);
  dccexProtocol.setDelegate(&dccexCallbacks);
  dccexProtocol.connect(&CLIENT);
  createMenus();
  currentMenuPtr->display();
}




void loop() {
  dccexProtocol.check();
  validateConnection();
  updateRoster();
  updateRoutes();
  updateTurnouts();
  updateTurntables();
  pot1.averageInput();
  pot2.averageInput();
  pot3.averageInput();
  newLoco1Speed = map(pot1.getAverage(), POT_MIN, POT_MAX, 0, 126);
  newLoco2Speed = map(pot2.getAverage(), POT_MIN, POT_MAX, 0, 126);
  newLoco3Speed = map(pot3.getAverage(), POT_MIN, POT_MAX, 0, 126);
  bool updateSpeeds = false;
  if (newLoco1Speed != loco1Speed) {
    loco1Speed = newLoco1Speed;
    updateSpeeds = true;
  }
  if (newLoco2Speed != loco2Speed) {
    loco2Speed = newLoco2Speed;
    updateSpeeds = true;
  }
  if (newLoco3Speed != loco3Speed) {
    loco3Speed = newLoco3Speed;
    updateSpeeds = true;
  }
  if (updateSpeeds) {
    currentMenuPtr->display();
  }
  char key = keypad.getKey();
  if (key) {
    currentMenuPtr->handleKeyPress(key);
  }
}




#if defined(ARDUINO_BLUEPILL_F103C8)
void disableJTAG() {


  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG);


}
#endif