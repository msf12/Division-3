#ifndef SETTINGS_H
#define SETTINGS_H

#include <SdFat.h>
#include <string>

#define SD_CS_PIN SS

//User Input pins
#define PREVIOUS_SONG 10
#define NEXT_SONG 10
#define PREVIOUS_MENU_ITEM 10
#define NEXT_MENU_ITEM 10
#define PLAY_SELECT 10
#define PREVIOUS_MENU 10
#define SWITCH_VIEW_MODE 10

const uint16_t PREVIOUS_SONG_DELAY = 1000;
bool songView = false;

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

#endif