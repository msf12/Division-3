#include "Settings.h"
#include "SongPlayer.h"

void setup()
{
	clearScreen();
	tft.setTextColor(ILI9341_BLACK);
	printToScreen("Booting...");
	if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
	{
		printToScreen("Storage error: MicroSD card could not be read\nReboot or replace the MicroSD card");
	}
	if(!songDatabaseExists())
	{
		printToScreen("Updating song database");
		rebuildSongDatabase();
	}
	else
	{
		printToScreen("Building song database");
		buildSongDatabase();
	}
	printToScreen("Device has booted successfully\nLoading main menu");
	menu = MAIN_MENU; //typedef enum MenuType

	//User Input interrupts
	attachInterrupt(PREVIOUS_SONG, previousSongInterrupt, RISING);
	attachInterrupt(NEXT_SONG, nextSongInterrupt, RISING);
	attachInterrupt(PREVIOUS_MENU_ITEM, previousItemInterrupt, LOW);
	attachInterrupt(NEXT_MENU_ITEM, nextItemInterrupt, LOW);
	attachInterrupt(PLAY_SELECT, playSelectInterrupt, LOW);
	attachInterrupt(PREVIOUS_MENU, previousMenuInterrupt, LOW);
	attachInterrupt(SWITCH_VIEW_MODE, switchViewInterrupt, LOW);
}

void loop()
{
	//print the current view to the screen
	displayMenu(menu);
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
	noInterrupts();
	pauseSong();
	longPress = true;
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
		if(nextSong != "")
		{
			playNewSong(nextSong);
		}
		else
		{
			stopPlaying();
		}
	}
	interrupts();
}

void forwardsTrackingInterrupt()
{
	noInterrupts();
	pauseSong();
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
		if(exists(previousItem))
		{
			selectedItem = previousItem;
		}
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
		if(exists(nextItem))
			selectedItem = nextItem;
	}
	interrupts();
}

void playSelectInterrupt()
{
	noInterrupts();
	//if the current view is the song view, pause the current song
	if(songView)
	{
		pauseSong();
	}
	//else if the current view is a list of songs play the selected new song
	//if the song is the same as the current song it will continue to play
	else if(menu == ARTIST_SONG_MENU || menu == ALBUM_SONG_MENU || menu == SONG_MENU)
	{
		playNewSong(selectedItem);
	}
	//else interpret the play button as the menu item select button and change to the selected submenu
	else
	{
		changeMenu(selectedItem);
	}
	interrupts();
}

void previousMenuInterrupt()
{
	noInterrupts();
	//if the current view is the song view, change to menu view
	if(songView)
	{
		changeToMenuView();
	}
	//else change to the menu one directory higher than the current menu
	else if(menu != MAIN_MENU)
	{
		changeMenu(previousMenu);
	}
	interrupts();
}

void switchViewInterrupt()
{
	noInterrupts();
	//if the current view is the song view, switch to the most recently viewed menu
	if(songView)
	{
		changeToMenuView();
	}
	//else switch to song view
	else
	{
		changeToSongView();
	}
	interrupts();
}