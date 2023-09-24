# DCC-EX Serial Throttle

A serial throttle using three linear potentiometers, an OLED, and a 4 x 3 keypad designed for use with DCC-EX EX-CommandStation.

## Hardware

- STM32F103C8 Bluepill
- SPI or I2C OLED
- 4 x 3 Keypad
- 3 x 10K linear potentiometers

## Pins

### SPI

- SS - PA4
- MOSI - PA7
- MISO - PA6
- SCK - PA5

### I2C

- SDA - PB7
- SCL - PB6

### Serial

- RX -PA10
- TX - PA9

(Write to serial pins with Serial1)
