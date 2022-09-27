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
#define OLED_TYPE &SH1106_128x64
#define CS_PIN  10
#define DC_PIN  9

// Comment out above and uncomment from the line below for I2C
// #define USE_OLED_I2C
// #define OLED_TYPE &Adafruit128x64
// #define OLED_ADDRESS 0x3C
// I2C definition ends here, don't uncomment this line

/////////////////////////////////////////////////////////////////////////////////////
//  Keypad configuration.
// 
#define KEYPAD_DEBOUNCE 10
#define KEYPAD_HOLD 500
#define KEYPAD_PIN1 3
#define KEYPAD_PIN2 4
#define KEYPAD_PIN3 5
#define KEYPAD_PIN4 6
#define KEYPAD_PIN5 7
#define KEYPAD_PIN6 8
#define KEYPAD_PIN7 12

/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer tuning.
//  Set the minimum and maximum voltages for the ADC.
//  Allows zero and full to stay away from potentially twitching end points of the pot.
// 
#define POT_MIN 0
#define POT_MAX 1020

#endif
