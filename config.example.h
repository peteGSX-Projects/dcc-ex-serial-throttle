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
//  OLED configuration.
//  Use this section for SPI, and the section further down for I2C.
//  Comment out the section not in use.
// 
// Uncomment from the line below for SPI
#define USE_OLED_SPI
#define CS_PIN  7
#define DC_PIN  9
#define OLED_FONT Callibri11

// Comment out above and uncomment from the line below for I2C
// #define USE_OLED_I2C
// #define OLED_ADDRESS 0x3C
// I2C definition ends here, don't uncomment this line

/////////////////////////////////////////////////////////////////////////////////////
//  Keypad configuration.
// 
#define KEYPAD_DEBOUNCE = 10;
#define KEYPAD_HOLD = 200;



/////////////////////////////////////////////////////////////////////////////////////
//  Potentiometer tuning.
//  Note reverse is... reversed. A higher value is closer to the centre of the slider
//  and therefore closer to 0.
// 
#define FWD_MIN 525
#define FWD_MAX 1020
#define REV_MIN 495
#define REV_MAX 3
#define DIR_SWAP 510

#endif
