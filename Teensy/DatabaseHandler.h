#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <string>

class DatabaseHandler
{
private:
	SdFat SD;

public:

	DatabaseHandler()
	{}

	virtual ~DatabaseHandler()
	{}

	SongInfo getSongInfo(string songName)
	{
	}

	DoublyLinkedList getSubMenu(string newMenu)
	{
	}
	
	bool setup()
	{
		if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
		{
			return false;
		}
	}

};

#endif // DATABASEHANDLER_H
