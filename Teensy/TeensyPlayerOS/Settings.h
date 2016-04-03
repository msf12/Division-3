#ifndef SETTINGS_H
#define SETTINGS_H

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

#define LINE_BUFFER_SIZE 50
#define CHARS_ALLOWED_IN_MEMORY 5000
// #define DATABASE_BLOCK_SIZE ### //number of lines per database block

#define LINES_ON_SCREEN 20

#include <SdFat.h>
SdFat SD;

#include <SPI.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include "DoublyLinkedList.h"

//MenuNavigationDirection represents whether the menu is traversed upwards or downwards
//This is passed to the UIHandler with a menu item to display
//The UIHandler adjusts its internal DoublyLinkedList of menu items appropriately
//adding the new item at the head or tail, updating the display, and removing the hidden item
typedef enum MenuNavigationDirection
{
	UP,
	DOWN
}MenuNavigationDirection;

// typedef enum menutype
// {
// 	MainMenu,
// 	Artist,
// 	Album,
// 	Song,
// 	ArtAlbum,
// 	AlbSong
// }Menu;

String MainMenu[] = {"Artists","Albums","Songs","Update song library"};

// Menu menu;

#include "UIHandler.h"
#include "DatabaseHandler.h"
#include "AudioHandler.h"
// #include "InputHandler.h"

UIHandler ui;
DatabaseHandler db;
AudioHandler audio;
// InputHandler input;

#endif