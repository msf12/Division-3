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

	uint16_t numSongs = 0, numAlbums = 0, numArtists = 0;

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

		populateTempFiles(SD.vwd());
		sortDatabase();

		return true;
	}

private:

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
	bool populateTempFiles(SdFile directory, String path = "")
	{
		SdFile file;
		while(file.openNext(directory,O_READ))
		{
			static char fileName[250];

			if(!file.isDir())
			{
				numSongs++;
				//reduced to accurate number in sorting
				numAlbums++;
				numArtists++;
				// songs.println(songTitle + \t + path + *fileNamePtr);
				// albums.println(albumTitle + \t + songTitle);
				// artists.println(artist + \t + albumTitle);
			}
			else
			{
				populateTempFiles(file, (path + fileName));
			}
		}
		return true;
	}

	bool sortDatabase()
	{
		//TODO: sort artists
		//TODO: create artist indexing table
		//TODO: sort albums
		//TODO: create album indexing table
		//TODO: sort songs

		//PSEUDOCODE

		SdFile file; //file is the current master database file (EX. artists)
		uint8_t tempCount = 0; //count of temporary database files created from the contents of file
		ifstream fin(file); //input stream for reading from file
		/*
		usage example

		const int line_buffer_size = 18;
		char buffer[line_buffer_size];
		ifstream sdin("getline.txt");
		int line_number = 0;
		
		while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
		  int count = sdin.gcount();
		  if (sdin.fail()) {
		    cout << "Partial long line";
		    sdin.clear(sdin.rdstate() & ~ios_base::failbit);
		  } else if (sdin.eof()) {
		    cout << "Partial final line";  // sdin.fail() is false
		  } else {
		    count--;  // Don’t include newline in count
		    cout << "Line " << ++line_number;
		  }
		  cout << " (" << count << " chars): " << buffer << endl;
		}
		*/

		//LOOP: for the entirety of file
		while(fin.getline())
		{
			//Struct is a placeholder name for a struct holding a single term and a single associated piece of information (EX. album - song)
			//list is a placeholder name for the list of Structs
			//mem is an uint16_t representing the amount of Structs that can be simultaneously held in memory
			DoublyLinkedList<Struct> list();

			for(uint16_t i = 0; i < mem; ++i)
			{
				Struct *s = new Struct();
				String info = buffer;
				s->field1 = info.substring(0,info.firstIndexOf("\t"));
				s->field2 = info.substring(info.firstIndexOf("\t")+1); //make sure not to include '\n'
				list.add(*s);
			}
			list.sort();

			//create a new temporary file with a default name (EX. tempartists3.db)
			SdFile tempFile("defaultname" + tempCount + ".db");
			
			for(DoublyLinkedList<Struct>::iterator i = list.begin(); i != list.end(); ++i)
			{
				//add the first and second fields of the Struct pointed at by i to the database file
				tempFile.print((*i)->field1 + "\t" + field2);

				//while the first field of the Struct pointed to by i is equal to
				//the first field of the Struct pointed at by i.next()
				//add the second field of i.next() to the current line of the sorted database
				while((*i)->field1 == (*(i.next()))->field1)
				{
					++i;
					tempFile.print("," + (*i)->);
				}
				tempFile.println();
			}
		}
		
		//ENDLOOP: loop through tempFiles in pairs and merge them, consolidating duplicates each time

		while(tempCount > 2)
		{
			uint8_t numPairs = tempCount/2; //number of temporary file pairs possible
			for(int i = 0; i < numPairs; ++i)
			{
				SdFile file1("defaultname" + i + ".db");
				SdFile file2("defaultname" + ++i + ".db");
				mergeFiles(file1,file2);
				tempCount-1;
			}
		}
		//create index file on the final merge
		//manually merge into new artists.db file
		//create artistsIndex.db after each artist is done
	}

};

#endif // DATABASEHANDLER_H
