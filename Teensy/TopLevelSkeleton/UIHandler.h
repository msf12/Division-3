#ifndef UIHANDLER_H
#define UIHANDLER_H

class UIHandler
{
private:
	ILI9341_t3 tft;

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

	// bool changeMenu(DoublyLinkedList menuItems)
	// {
	// }

	bool clearScreen()
	{
		tft.fillScreen(ILI9341_WHITE);
		tft.setCursor(0, 0);
		return true;
	}

	bool printToScreen(String s)
	{
		tft.println(s);
		return true;
	}

	bool setup()
	{
		tft.setTextColor(ILI9341_BLACK);
		tft.setTextSize(2);
		clearScreen();
		return true;
	}

	bool newMenu(DoublyLinkedList<String> newItems)
	{
		//todo: replace current internal list with visible elements of new list
		return false;
	}

	bool scrollMenu(MenuNavigationDirection navDirect)
	{
		//todo: update highlighted menu item
		//this method is called in the case where the menu is navigated in a direction
		//and there are no more elements to display in that direction
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

};

#endif // UIHANDLER_H
