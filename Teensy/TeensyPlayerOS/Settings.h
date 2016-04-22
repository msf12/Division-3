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

//Input Handling type (0 is false)
//CURRENTLY UNUSED
#define USE_INTERRUPTS 1

// For optimized ILI9341_t3 library (see: https://github.com/PaulStoffregen/ILI9341_t3)
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

//The size of the buffer when reading lines from files
#define LINE_BUFFER_SIZE 50
//The amount of characters that can be held in memory simultaneously
//The number of chars read will exceed this number by up to LINE_BUFFER_SIZE-1
//due to the way characters are read
#define CHARS_ALLOWED_IN_MEMORY 5000

//The number of lines that can fit on the screen being used
#define LINES_ON_SCREEN 20

//https://github.com/greiman/SdFat
#include <SdFat.h>
SdFat SD;

//misc dependencies
#include <SPI.h>
#include <font_Arial.h>
#include <font_ArialBold.h>

#include "DoublyLinkedList.h"

//Enum to represent the class of the current menu
//CURRENTLY UNUSED
// typedef enum menutype
// {
// 	MainMenu,
// 	Artist,
// 	Album,
// 	Song,
// 	ArtAlbum, //albums by selected artist
// 	AlbSong //songs on selected album
// }Menu;

// Menu menu;

//hard coded main menu options and MenuNavigationDirection for demo purposes
//CURRENTLY UNFINISHED
String MainMenu[] = {"Artists","Albums","Songs","Update song library"};

enum MenuNavigationDirection
{
	UP,
	DOWN
};

//OS modules by order they are laoded
//SOME UNUSED AND UNFINISHED
#include "UIHandler.h"
#include "DatabaseHandler.h"
#include "AudioHandler.h"
// #include "InputHandler.h"

UIHandler ui;
DatabaseHandler db;
AudioHandler audio;
// InputHandler input;

#endif