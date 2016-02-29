#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <string>

class UIHandler
{
private:
	ILI9341_t3 tft;

public:

	UIHandler()
	{
		tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
		tft.begin();
	}

	virtual ~UIHandler()
	{}

	bool songView(SongInfo newSong)
	{
	}

	bool changeMenu(DoublyLinkedList menuItems)
	{
	}

	bool clearScreen()
	{
		tft.fillScreen(ILI9341_WHITE);
		tft.setCursor(0, 0);
		return true;
	}

	bool printToScreen(string s)
	{
		tft.println(s);
		return true;
	}

	bool setup()
	{
		tft.setTextColor(ILI9341_BLACK);
		clearScreen();
	}

};

#endif // UIHANDLER_H
