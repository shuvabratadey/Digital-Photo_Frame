-----------------------------------------------------------------
| ST7735 Pin | ESP32 Pin (Example)        | Description          |
| ---------- | -------------------------- | -------------------- |
| VCC        | 3.3V                       | Power                |
| GND        | GND                        | Ground               |
| RESET      | GPIO 4  yellow             | Reset                |
| DC (RS/A0) | GPIO 16 green              | Data/Command         |
| SDA (MOSI) | GPIO 23 blue               | SPI MOSI             |
| SCL        | GPIO 18  blue              | SPI Clock            |
-----------------------------------------------------------------
######################################
Display ➡️ 1.8" 128x160 (WIDTH x HIGHT)
Display Link: https://robokits.co.in/displays/lcd-display/1.8-inch-st7735-tft-lcd-module-with-4-io-128160?srsltid=AfmBOooSXd-szoii0g_ixwxS0nMvp50dmn3h7G07zG9mc3wdTbqmyyKS9KM

Goto ==> C:\Users\dell\Documents\Arduino\libraries\TFT_eSPI-master\User_Setup.h
And change this lines ==>

#define ST7735_DRIVER
#define TFT_WIDTH  130
#define TFT_HEIGHT 161
#define ST7735_BLACKTAB

// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP   ######

// For ESP32 Dev board (only tested with ILI9341 display)
// The hardware SPI can be mapped to any pins

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   -1  // Chip select control pin
#define TFT_DC    16  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)

Goto Examples\TFT_eSPI-master\examples\160 x 128\TFT_graphicstest_small

***************************
SCL ==> GPIO 22 green
SDA ==> GPIO 21 yellow
***************************

Up button GPIO_32
Down button GPIO_33
Middle Button GPIO_13 (T4)

Haptic motor GPIO_26
--------------------------------------------------------