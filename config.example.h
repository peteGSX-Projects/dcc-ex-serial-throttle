/*
 * This is an example config.h file for the DCC-EX serial throttle.
*/

/////////////////////////////////////////////////////////////////////////////////////
//  Ensure only one instance of config.h is loaded.
// 
#ifndef CONFIG_H
#define CONFIG_H

/////////////////////////////////////////////////////////////////////////////////////
//  Number of times to sample the analogue input to average out twitchy sliders.
// 
#define SAMPLES 10

/////////////////////////////////////////////////////////////////////////////////////
//  ESTOP button pin settings.
// 
#define ESTOP_PIN 2
#define ESTOP_POLARITY LOW
#define ESTOP_DEBOUNCE 30

/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer analog pins.
// 
#define POT1_PIN A0
#define POT2_PIN A1
#define POT3_PIN A2

/////////////////////////////////////////////////////////////////////////////////////
//  OLED configuration.
//  Use this section for SPI, and the section further down for I2C.
//  Comment out the section not in use.
// 
// Common OLED settings
#define OLED_FONT Callibri11

// Uncomment from the line below for SPI
#define USE_OLED_SPI
#define CS_PIN  10
#define DC_PIN  9

// Comment out above and uncomment from the line below for I2C
// #define USE_OLED_I2C
// #define OLED_ADDRESS 0x3C
// I2C definition ends here, don't uncomment this line

/////////////////////////////////////////////////////////////////////////////////////
//  Keypad configuration.
// 
#define KEYPAD_DEBOUNCE 10
#define KEYPAD_HOLD 200
#define KEYPAD_PIN1 3
#define KEYPAD_PIN2 4
#define KEYPAD_PIN3 5
#define KEYPAD_PIN4 6
#define KEYPAD_PIN5 7
#define KEYPAD_PIN6 8
#define KEYPAD_PIN7 12

/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer tuning.
//  Note reverse is... reversed. A higher value is closer to the centre of the slider
//  and therefore closer to 0.
// 
//  A neutral area around the centre is recommended to provide a positive stop area.
//  Set DIR_SWAP to be the point at which direction changes within this neutral area.
// 
#define FWD_MIN 525
#define FWD_MAX 1020
#define REV_MIN 495
#define REV_MAX 3
#define DIR_SWAP 510

/////////////////////////////////////////////////////////////////////////////////////
//  Debug options
// 
#define DEBUG

#endif
