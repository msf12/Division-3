#ifndef SETTINGS_H
#define SETTINGS_H

#include <SdFat.h>
#include <SPI.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include "AudioHandler.h"
#include "DatabaseHandler.h"
#include "InputHandler.h"
#include "UIHandler.h"

SdFat SD;
UIHandler ui;
DatabaseHandler db;
AudioHandler audio;
InputHandler input;

#define SD_CS_PIN SS

//User Input pins
#define PREVIOUS_SONG 2
#define NEXT_SONG 3
#define PREVIOUS_MENU_ITEM 4
#define NEXT_MENU_ITEM 5
#define PLAY_SELECT 23
#define PREVIOUS_MENU 22
#define SWITCH_VIEW_MODE 10

//Input Handling type
#define USE_INTERRUPTS 1

// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

typedef struct {
	
} SongInfo;

#endif