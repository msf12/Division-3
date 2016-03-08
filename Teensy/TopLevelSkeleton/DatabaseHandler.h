#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

extern SdFat SD;

class DatabaseHandler
{
	SdFile artists; //file containing alphabetic list of artists and the location of their associated albums in alb.it 
	SdFile artistsIndex; //file containing alphabetic list of artists and their locations in art.db
	SdFile albums; //file containing alphabetic list of albums and the location of their associated songs in songs.db 
	SdFile albumsIndex; //file containing alphabetic list of albums and their locations in alb.it
	SdFile songs; //file containing alphabetic list of songs

public:

	DatabaseHandler()
	{}

	virtual ~DatabaseHandler()
	{}

	// SongInfo getSongInfo(String songName)
	// {
	// }

	// DoublyLinkedList getSubMenu(String newMenu)
	// {
	// }
	
	bool setup()
	{
		if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
		{
			return false;
		}

		if (!artists.open("art.db", O_RDWR | O_CREAT | O_AT_END) || 
			!artistsIndex.open("art.it", O_RDWR | O_CREAT | O_AT_END) || 
			!albums.open("alb.db", O_RDWR | O_CREAT | O_AT_END) || 
			!albumsIndex.open("alb.it", O_RDWR | O_CREAT | O_AT_END) || 
			!songs.open("songs.db", O_RDWR | O_CREAT | O_AT_END))
		{
		    return false;
		}

		String currDir = "/Music/";
		SD.chdir(currDir.c_str());

		//SD.vwd() returns an SdBaseFile
		//Thus I should be able to iterate through subdirectories recursively using the directory file as a parameter
		//This will take time and memory so it should only be run if the database is not currently populated or if the user says to
		//This requires a "Rebuild Song Database" option in the main menu
		//In the future this should be modified to determine the delta between the database and the filesystem and adjust appropriately
		/*
		while(tempFile.openNext(baseFileOfDir,O_READ))
		{
			if(tempFile.isSong())
			{
				songs.println(songTitle);
				albums.println(albumTitle + \t + songTitle);
				artists.println(artist + \t + albumTitle);
			}
			else //tempFile is dir
			{
				recur(tempFile)
			}
		}
		*/

		SdFile file;

		while(file.openNext(SD.vwd(),O_READ))
		{
			static char fileName[250];
			static char *fileNamePtr;
			file.getName(fileName,250);
			fileNamePtr = new char[strlen(fileName)+1];
			strcpy(fileNamePtr,fileName);
			artistsIndex.println(*fileNamePtr);
		}

		return true;
	}

};

#endif // DATABASEHANDLER_H
