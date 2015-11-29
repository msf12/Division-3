#ifndef SETTINGS_H
#define SETTINGS_H

#include <SdFat.h>
#include <string>

#define SD_CS_PIN SS
#define PREVIOUS_SONG_DELAY 1000

typedef enum MenuTypeEnum {
	MAIN_MENU,
	ARTIST_MENU,
	ARTIST_ALBUM_MENU,
	ARTIST_SONG_MENU,
	ALBUM_MENU,
	ALBUM_SONG_MENU,
	SONG_MENU,
	SONG_VIEW,
	SETTINGS_MENU
} MenuType;

typedef enum ButtonEnum {
	PREVIOUS_SONG,
	NEXT_SONG,
	PREVIOUS_MENU_ITEM,
	NEXT_MENU_ITEM,
	PLAY_SELECT,
	PREVIOUS_MENU,
	NOW_PLAYING
} Button;

SdFat SD;
MenuType menu;

#endif