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
		tft.begin();
	}

	virtual ~UIHandler()
	{}

	// bool songView(SongInfo newSong)
	// {
	// }

	bool clearScreen()
	{
		tft.fillScreen(ILI9341_WHITE);
		tft.setCursor(0, 0);
		return true;
	}

	bool print(String s)
	{
		return tft.print(s);
	}

	bool println(String s)
	{
		return print(s + '\n');
	}

	bool setup()
	{
		tft.setTextColor(ILI9341_BLACK);
		tft.setTextSize(2);
		clearScreen();
		return true;
	}

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
		//todo: add newItem to tail of visible items if navDirect == DOWN or
		//to head if navDirect == UP
		//update display (center item remains highlighted)
		//remove hidden item from list of visible items
		return false;
	}

	void textWrap(bool b)
	{
		tft.setTextWrap(b);
	}

};

#endif // UIHANDLER_H
