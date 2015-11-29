#include "Settings.h"

bool checkForInput();
Button getInput();
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
}

void loop()
{
	//print the current view to the screen
	displayMenu(menu);

	//getInput waits until the input buffer is not empty
	static Button pressed;
	pressed = getInput(); //typedef enum Button

	//switches based on which button was pressed
	switch (pressed) {

		//if the previous song button was pressed
		case PREVIOUS_SONG:

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
					if((pressed = getInput()) == PREVIOUS_SONG)
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
			//if no second button was pressed break the switch statement
			if(pressed == PREVIOUS_SONG)
				break;

		//if the next song button was pressed
		case NEXT_SONG:
			//if the next song exists play it otherwise stop playing music altogether
			if(nextSong != "")
				playNewSong(nextSong);
			else
				stopPlaying();
			break;

		//if the previous menu item button was pressed
		case PREVIOUS_MENU_ITEM:
			//if the current view displays a menu
			if(menu != SONG_VIEW)
			{	
				//if there is a previous menu item update which item is selected
				if(exists(previousItem))
					selectedItem = previousItem;
			}
			//else do nothing
			break;

		//if the next menu item button was pressed
		case NEXT_MENU_ITEM:
			//if the current view displays a menu
			if(menu != SONG_VIEW)
			{
				//if there is a next menu item update which item is selected
				if(exists(nextItem))
					selectedItem = nextItem;
			}
			//else do nothing
			break;

		//if the play/select button was pressed
		case PLAY_SELECT:
			//if the current view is the song view, pause the current song
			if(menu == SONG_VIEW)
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
			break;
		//if the previous menu button was pressed
		case PREVIOUS_MENU:
			//if the current view is the song view, change to the appropriate menu view
			if(menu == SONG_VIEW)
			{
				changeToMenuView(previousMenu);
			}
			//else change to the menu one directory higher than the current menu
			else if(menu != MAIN_MENU)
			{
				changeMenu(previousMenu);
			}
			break;
		//if the now playing button was pressed
		case NOW_PLAYING:
			//if the current view is the song view, switch to the most recently viewed menu
			if(menu == SONG_VIEW)
			{
				changeToMenuView();
			}
			//else switch to song view
			else
			{
				changeToSongView();
			}
			break;
		//default means an error has occured
		default:
			eraseScreen();
			printToScreen("Button Error: unknown button pressed");
	}
}