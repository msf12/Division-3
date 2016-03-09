#include "Settings.h"

/*
TODO:
store info of currently displayed files
main menu?
interpret and handle inputs
*/

void setup()
{
	while(!Serial);
	ui.setup();
	ui.printToScreen("Screen connected");
	
	ui.printToScreen("Building song database");
	db.setup();

	ui.printToScreen("Initializing audio device");
// 	audio.setup();
	
// 	ui.printToScreen("Preparing user input");

// 	ui.printToScreen("Device has booted successfully\nLoading main menu");
// 	menu = MAIN_MENU; //typedef enum MenuType

// #if USE_INTERRUPTS
// 	ui.printToScreen("Using interrupts for button input");
// #else
// 	ui.printToScreen("Using polling for button input");
// #endif

// 	userInputSetup();
}

void loop()
{
}