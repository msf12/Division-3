#include "Settings.h"

/*
 * DEVELOPER'S NOTE:
 * The serial connection and ui print statements in setup()
 * are there for debugging purposes and should be removed
 * in a final build.
*/
void setup()
{

	//wait for serial connection to boot
	while(!Serial);

	ui.setup();
	ui.println("Screen connected");
	delay(1000);
	
	ui.println("Locating song database");
	delay(1000);
	uint8_t dbfailed = db.setup();
	if(dbfailed)
	{
		if(dbfailed == 2)
		{
			ui.println("ERROR: MicroSD card not found!!!");
			while(1);

		}
		
		ui.println("Database not found and was rebuilt");
		delay(1000);
	}
	else
	{
		ui.println("Database located");
		delay(1000);
	}

	ui.println("Initializing audio device");
	delay(1000);
	audio.setup();
	
	//USER INPUT NOT YET USED

	// ui.println("Preparing user input");

// #if USE_INTERRUPTS
// 	ui.println("Using interrupts for button input");
// #else
// 	ui.println("Using polling for button input");
// #endif

// 	userInputSetup();
	ui.println("Device has booted successfully\nLoading main menu");
	delay(1000);
	
	//disable textWrap so long names won't cause issues
	ui.textWrap(false);
	//printMenu is a demo method as the UI handler has not been implemented yet
	ui.printMenu(MainMenu,4);	
}

/*
 * The current OS loop is to demo
 */
void loop()
{
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(DOWN);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
	ui.scrollMenu(UP);
	delay(1000);
}