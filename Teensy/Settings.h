
#ifndef SETTINGS_H
#define SETTINGS_H

#include <SdFat.h>
#include <string.h>
#include <SPI.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include <ILI9341_t3.h>

#define SD_CS_PIN SS

//User Input pins
#define PREVIOUS_SONG 10
#define NEXT_SONG 10
#define PREVIOUS_MENU_ITEM 10
#define NEXT_MENU_ITEM 10
#define PLAY_SELECT 10
#define PREVIOUS_MENU 10
#define SWITCH_VIEW_MODE 10

// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

uint16_t PREVIOUS_SONG_DELAY = 1000;
bool songView = false;
bool longPress = false;

typedef enum MenuTypeEnum {
	MAIN_MENU,
	ARTIST_MENU,
	ARTIST_ALBUM_MENU,
	ARTIST_SONG_MENU,
	ALBUM_MENU,
	ALBUM_SONG_MENU,
	SONG_MENU,
	SETTINGS_MENU
} MenuType;

// typedef enum ButtonEnum {
// 	PREVIOUS_SONG,
// 	NEXT_SONG,
// 	PREVIOUS_MENU_ITEM,
// 	NEXT_MENU_ITEM,
// 	PLAY_SELECT,
// 	PREVIOUS_MENU,
// 	NOW_PLAYING
// } Button;

SdFat SD;
MenuType menu;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
String previousSong, nextSong, previousMenu, nextMenu, previousItem, nextItem, selectedItem;

inline void clearScreen()
{
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(0, 0);
}

inline void printToScreen(String s)
{
	tft.println(s);
}

bool checkForInput();
int getInput();
void eraseScreen();
void printToScreen(String s);
void displayMenu(MenuType menu);
bool songDatabaseExists();
void buildSongDatabase();
void rebuildSongDatabase();
void playNewSong(String s);
void stopPlaying();
void pauseSong();
void restartCurrentSong();
void changeMenu(String s);
void changeToMenuView();
void changeToMenuView(String s);
void changeToSongView();
bool exists(String s);

#endif