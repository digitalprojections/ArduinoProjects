#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>



int checkInput();
void ClearScreen();
void IllegalMove();
void CountDown(int c);
void UpdateGameStats(uint8_t livesLeft, uint8_t hitCount, uint8_t missCount, int8_t wavesLeft = -1);