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
#define SLIDER_SAMPLES 10

/////////////////////////////////////////////////////////////////////////////////////
//  Number of times to retry connecting at startup before reporting connection error.
//  A connection retry occurs every second, and can take some time if using WiFi on
//  a CommandStation as it takes a few seconds at least to connect to WiFi.
// 
#define CONNECT_RETRIES 5

/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer analog pins.
// 
#define POT1_PIN PA0
#define POT2_PIN PA1
#define POT3_PIN PA2

/////////////////////////////////////////////////////////////////////////////////////
//  OLED configuration.
//  Use this section for SPI, and the section further down for I2C.
//  Comment out the section not in use.
// 
// Common OLED settings
#define OLED_FONT System5x7

// Uncomment from the line below for SPI
#define USE_OLED_SPI
#define OLED_TYPE &SH1106_128x64
#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
#define CS_PIN  PA4
#define DC_PIN  PA3
#else
#define CS_PIN  1
#define DC_PIN  3
#endif
// SCK - PA5
// MISO - PA6
// MOSI - PA7

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

#if defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F411CE)
#define KEYPAD_PIN1 PB3
#define KEYPAD_PIN2 PB4
#define KEYPAD_PIN3 PB5
#define KEYPAD_PIN4 PB6
#define KEYPAD_PIN5 PB7
#define KEYPAD_PIN6 PB8
#define KEYPAD_PIN7 PB9
#else
#define KEYPAD_PIN1 19
#define KEYPAD_PIN2 18
#define KEYPAD_PIN3 17
#define KEYPAD_PIN4 16
#define KEYPAD_PIN5 4
#define KEYPAD_PIN6 0
#define KEYPAD_PIN7 2
#endif

/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer tuning.
//  Set the minimum and maximum voltages for the ADC.
//  Allows zero and full to stay away from potentially twitching end points of the pot.
// 
#define POT_MIN 0
#define POT_MAX 1020

#endif
