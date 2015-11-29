#include "Settings.h"

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

String previousSong, nextSong, previousMenu, nextMenu, previousItem, nextItem, selectedItem;

void setup()
{
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
	attachInterrupt(PREVIOUS_SONG, previousSongInterrupt, LOW);
	attachInterrupt(NEXT_SONG, nextSongInterrupt, LOW);
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

//PREVIOUS SONG NEEDS DOUBLE CLICK FIXED
void previousSongInterrupt()
{
	//two presses in PREVIOUS_SONG_DELAY milliseconds means previous song
	//a single press means restart the current song
	static unsigned long timer;
	timer = millis();

	//check for the PREVIOUS_SONG button to be pressed again within PREVIOUS_SONG_DELAY milliseconds
	while(timer+PREVIOUS_SONG_DELAY < millis())
	{
		//checks the input buffer for a button press
		if(checkForInput())
		{
			//get the button that was pressed and update pressed
			if(getInput() == PREVIOUS_SONG)
			{
				//if the button was PREVIOUS_SONG play the previous song
				//if the previous song does not exist stop playing altogether
				if(exists(previousSong))
					playNewSong(previousSong);
				else
					stopPlaying();
				break;
			}
		}
	}
	//if PREVIOUS_SONG was not pressed twice in time restart the current song
	restartCurrentSong();
}

void nextSongInterrupt()
{
	noInterrupts();
	//if the next song exists play it otherwise stop playing music altogether
	if(nextSong != "")
		playNewSong(nextSong);
	else
		stopPlaying();
	interrupts();
}

void previousItemInterrupt()
{
	noInterrupts();
	//if the current view displays a menu
	if(!songView)
	{	
		//if there is a previous menu item update which item is selected
		if(exists(previousItem))
			selectedItem = previousItem;
	}
	interrupts();
}

void nextItemInterrupt()
{
	noInterrupts();
	//if the current view displays a menu
	if(!songView)
	{
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