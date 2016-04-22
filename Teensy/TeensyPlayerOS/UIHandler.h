#ifndef UIHANDLER_H
#define UIHANDLER_H

class UIHandler
{
private:
	ILI9341_t3 tft;
	uint16_t selectedItem;
	String *currMenu;
	int currMenuSize;

public:

	UIHandler() : tft(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO)
	{
		//connect to the ILI9341_t3 display
		tft.begin();
	}

	virtual ~UIHandler()
	{}

	// bool songView(SongInfo newSong)
	// {
	// }

	//paint the screen white and reset the cursor position
	bool clearScreen()
	{
		tft.fillScreen(ILI9341_WHITE);
		tft.setCursor(0, 0);
		return true;
	}

	//print a single string
	bool print(String s)
	{
		return tft.print(s);
	}

	//print a single string followed by a newline
	bool println(String s)
	{
		return print(s + '\n');
	}

	bool setup()
	{
		//initialize the UI handler to print black text of a set size
		tft.setTextColor(ILI9341_BLACK);
		tft.setTextSize(2);

		//clear the screen
		clearScreen();
		return true;
	}

	//FOLLOWING METHODS FOR DEMO PURPOSES ONLY
	bool printMenu(String *menu, int menuSize, uint16_t index = 0)
	{
		selectedItem = index;
		currMenu = menu;
		currMenuSize = menuSize;
		clearScreen();

		for(int i = 0; i < menuSize; ++i)
		{
			if(i == selectedItem)
			{
				println("*" + menu[i]);
			}
			else
			{
				println(menu[i]);
			}
		}
		return true;
	}

	//this method is called in the case where the menu is navigated in a direction
	//and there are no more elements to display in that direction
	bool scrollMenu(MenuNavigationDirection navDirect)
	{
		if(navDirect == UP && selectedItem > 0)
		{
			printMenu(currMenu,currMenuSize,selectedItem-1);
		}
		else if(navDirect == DOWN && selectedItem < currMenuSize-1)
		{
			printMenu(currMenu,currMenuSize,selectedItem+1);
		}
		
		return false;
	}

	bool scrollMenu(MenuNavigationDirection navDirect, String newItem)
	{
		return false;
	}

	void textWrap(bool b)
	{
		tft.setTextWrap(b);
	}

};

#endif // UIHANDLER_H
