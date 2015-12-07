#include "Settings.h"
#include "UserInput.h"
#include "SongPlayer.h"

MenuType menu;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

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

	printToScreen("Initializing audio device");
	audioSetup();
	
	printToScreen("Preparing user input");
	userInputSetup();
}

void loop()
{
	//print the current view to the screen
	// displayMenu(menu);
	// Serial.println(digitalRead(PLAY_SELECT));
}