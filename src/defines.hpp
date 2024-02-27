#ifndef _DEFINES_H_
#define _DEFINES_H_

// DRV8825 stepper motor driver
#define DRV8825_EN_GPIO     17  // RASPI pin 11
#define DRV8825_M0_GPIO     27  // RASPI pin 13
#define DRV8825_M1_GPIO     22  // RASPI pin 15
#define DRV8825_M2_GPIO     23  // RASPI pin 16
#define DRV8825_RESET_GPIO  24  // RASPI pin 18
#define DRV8825_SLEEP_GPIO  25  // RASPI pin 22
#define DRV8825_STEP_GPIO   5   // RASPI pin 29
#define DRV8825_DIR_GPIO    6   // RASPI pin 31

// WS2812 Addressable LED array
#define BLACK 0x00000000
#define WHITE 0xffffffff
#define RED 0x00ff0000
#define GREEN 0x0000ff00
#define BLUE 0x000000ff
#define YELLOW 0x00ffff00

#define LED_STRING_FREQUENCY 800000
#define LED_STRING_PIXELS 10
#define WSS2812_DATA_GPIO 18    // RASPI pin 12 (PCM CLK)

#endif