#include "Settings.h"
#include "SongPlayer.h"

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

void setup()
{
	tft.begin();
	while(!Serial);
	clearScreen();
	tft.setTextColor(ILI9341_BLACK);
	printToScreen("Booting...");
	if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
	{
		printToScreen("Storage error: MicroSD card could not be read\nReboot or replace the MicroSD card");
	}
	// if(!songDatabaseExists())
	// {
		printToScreen("Updating song database");
	// 	rebuildSongDatabase();
	// }
	// else
	// {
		printToScreen("Building song database");
	// 	buildSongDatabase();
	// }
	printToScreen("Device has booted successfully\nLoading main menu");
	menu = MAIN_MENU; //typedef enum MenuType

	pinMode(PREVIOUS_SONG, INPUT_PULLUP);
	pinMode(NEXT_SONG, INPUT_PULLUP);
	pinMode(PREVIOUS_MENU_ITEM, INPUT_PULLUP);
	pinMode(NEXT_MENU_ITEM, INPUT_PULLUP);
	pinMode(PLAY_SELECT, INPUT_PULLUP);
	pinMode(PREVIOUS_MENU, INPUT_PULLUP);

	//User Input interrupts
	attachInterrupt(PREVIOUS_SONG, previousSongInterrupt, RISING);
	attachInterrupt(NEXT_SONG, nextSongInterrupt, RISING);
	// attachInterrupt(PREVIOUS_SONG, backwardsTrackingInterrupt, LOW);
	// attachInterrupt(NEXT_SONG, forwardsTrackingInterrupt, LOW);
	attachInterrupt(PREVIOUS_MENU_ITEM, previousItemInterrupt, FALLING);
	attachInterrupt(NEXT_MENU_ITEM, nextItemInterrupt, FALLING);
	attachInterrupt(PLAY_SELECT, playSelectInterrupt, FALLING);
	attachInterrupt(PREVIOUS_MENU, previousMenuInterrupt, FALLING);
	// attachInterrupt(SWITCH_VIEW_MODE, switchViewInterrupt, FALLING);
}

void loop()
{
	//print the current view to the screen
	// displayMenu(menu);
	// Serial.println(digitalRead(PLAY_SELECT));
}

void previousSongInterrupt()
{
	noInterrupts();
	if(longPress)
	{
		longPress = false;
	}
	else
	{
		Serial.println("Previous song");
		/*
		if(current song has been playing for longer that 2 seconds)
		{
			restartCurrentSong();
		}
		else if(previousSong)
		{
			playNewSong(previousSong);
		}
		else
		{
			stopPlaying();
		}
		*/
	}
	interrupts();
}

void backwardsTrackingInterrupt()
{
	int time = millis();
	while(millis()-time < 1000);
	if(digitalRead(PREVIOUS_SONG) != LOW)
	{
		return;
	}
	noInterrupts();
	// pauseSong();
	while(digitalRead(PREVIOUS_SONG) == LOW)
	{
		if(!(millis()%1000))
		{
			Serial.println("Tracking backwards");
		}
	}
	interrupts();
}

void nextSongInterrupt()
{
	noInterrupts();
	if(longPress)
	{
		longPress = false;
	}
	else
	{
		Serial.println("Next song");
		//if the next song exists play it otherwise stop playing music altogether
		/*
		if(nextSong != "")
		{
			playNewSong(nextSong);
		}
		else
		{
			stopPlaying();
		}
		*/
	}
	interrupts();
}

void forwardsTrackingInterrupt()
{
	noInterrupts();
	// pauseSong();
	longPress = true;
	while(digitalRead(NEXT_SONG) == LOW)
	{
		if(!(millis()%1000))
		{
			Serial.println("Tracking forwards");
		}
	}
	interrupts();
}

void previousItemInterrupt()
{
	noInterrupts();
	//if the current view displays a menu
	if(!songView)
	{
		Serial.println("Previous menu item");
		//if there is a previous menu item update which item is selected
		/*
		if(exists(previousItem))
		{
			selectedItem = previousItem;
		}
		*/
	}
	interrupts();
}

void nextItemInterrupt()
{
	noInterrupts();
	//if the current view displays a menu
	if(!songView)
	{
		Serial.println("Next menu item");
		//if there is a next menu item update which item is selected
		// if(exists(nextItem))
		// 	selectedItem = nextItem;
	}
	interrupts();
}

void playSelectInterrupt()
{
	noInterrupts();
	if(millis()-lastInterrupt < 100)
		return;
	lastInterrupt = millis();
	Serial.println("Play/Select");
	//if the current view is the song view, pause the current song
	// if(songView)
	// {
		// pauseSong();
	// }
	//else if the current view is a list of songs play the selected new song
	//if the song is the same as the current song it will continue to play
	// else if(menu == ARTIST_SONG_MENU || menu == ALBUM_SONG_MENU || menu == SONG_MENU)
	// {
	// 	playNewSong(selectedItem);
	// }
	//else interpret the play button as the menu item select button and change to the selected submenu
	// else
	// {
	// 	changeMenu(selectedItem);
	// }
	interrupts();
}

void previousMenuInterrupt()
{
	noInterrupts();
	Serial.println("Previous menu");
	//if the current view is the song view, change to menu view
	// if(songView)
	// {
	// 	changeToMenuView();
	// }
	//else change to the menu one directory higher than the current menu
	// else if(menu != MAIN_MENU)
	// {
	// 	changeMenu(previousMenu);
	// }
	interrupts();
}

// void switchViewInterrupt()
// {
// 	noInterrupts();
// 	//if the current view is the song view, switch to the most recently viewed menu
// 	if(songView)
// 	{
// 		changeToMenuView();
// 	}
// 	//else switch to song view
// 	else
// 	{
// 		changeToSongView();
// 	}
// 	interrupts();
// }