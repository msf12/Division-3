#include "Settings.h"
#include "DoublyLinkedList.h"

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
	
	// ui.println("Preparing user input");


// #if USE_INTERRUPTS
// 	ui.println("Using interrupts for button input");
// #else
// 	ui.println("Using polling for button input");
// #endif

// 	userInputSetup();
	ui.println("Device has booted successfully\nLoading main menu");
	delay(1000);
	
	ui.textWrap(false);
	ui.printMenu(MainMenu,4);	
}

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