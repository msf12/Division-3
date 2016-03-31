#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

extern SdFat SD;

class DatabaseHandler
{
	uint16_t numSongs = 0, numAlbums = 0, numArtists = 0;

	bool populateInitDatabase(FatFile *directory, String path = "");

	//splits the database into temp files and then merges the files in pairs
	//takes optional arguments for field delimiter
	bool sortDatabase(String database, const char delim = '\t');

	//reads a subset of database determined by memory capacity into a DoublyLinkedList
	//subsequently sorts the list, iterates through it to merge entries with duplicate primary fields separated by delim
	//and concatenate secondary fields using conn as the separator
	//and exports it into a temp file
	//returns the count of temporary files
	uint16_t splitDatabase(String &database, const char delim = '\t');

	//merges two files
	//returns a reference to the mergeCount for use by larger programs can update it
	//so that mergeFiles will not overwrite externally created merge files
	uint16_t& mergeFiles(const String &f1, const String &f2, const char delim = '\t', const String mergeFileName = "merge.db");

	uint16_t generateIDFiles(const String &database, const char delim = '\t');

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

		FatFile *musicRoot = new FatFile("/Music/", O_READ);

		populateInitDatabase(musicRoot);
		return true;
	}


};

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
bool DatabaseHandler::populateInitDatabase(FatFile *directory, String path)
{
	ofstream db("/init.db");
	char fileName[250];

	for(SdFile file; file.openNext(directory,O_READ);)
	{
		if(!file.isDir())
		{
			file.getName(fileName,250);
			numSongs++;
			//reduced to accurate number in sorting
			numAlbums++;
			numArtists++;
			// db << artist << '\t' << album << '\t' << song << '\n' << path + String(fileName) << endl;
			// songs.println(songTitle + \t + path + *fileNamePtr);
			// albums.println(albumTitle + \t + songTitle);
			// artists.println(artist + \t + albumTitle);
		}
		else
		{
			FatFile *subDir = new FatFile(fileName,O_READ);
			populateInitDatabase(subDir, (path + fileName));
		}
	}
	return true;
}

//splits the database into temp files and then merges the files in pairs
//takes optional arguments for primary field delimiter and secondary field list separators
bool DatabaseHandler::sortDatabase(String database, const char delim)
{
	const uint16_t tempFileCount = splitDatabase(database,delim);
	uint16_t mergeCount = 0;
	SdFile fileToRename;
	for (uint16_t i = 0; i < tempFileCount-1; i+=2)
	{
		mergeFiles(String("temp")+String(i)+".db",
			String("temp")+String(i+1)+".db",
			delim,
			String("merge")+String(mergeCount++)+".db");
		
		SD.remove((String("temp")+String(i)+".db").c_str());
		SD.remove((String("temp")+String(i+1)+".db").c_str());
	}

	//if there are an odd number of temporary files
	//rename the final temporary file to a merge file for automated merging later
	//this merge file will always have the name merge[tempFileCount/2].db
	//this results from every pair of temp files being merged (tempFileCount/2 merge files)
	//and file names starting at merge0.db 
	if(tempFileCount%2)
	{
		fileToRename.open((String("temp")+String(tempFileCount-1)+".db").c_str(), O_WRITE | O_CREAT);
		fileToRename.rename(SD.vwd(),(String("merge")+String(mergeCount++)+".db").c_str());
	}

	for (uint16_t i = 0; i < mergeCount-1; i+=2)
	{
		mergeFiles(String("merge")+String(i)+".db",
			String("merge")+String(i+1)+".db",
			delim,
			String("merge")+String(mergeCount++)+".db");
		
		SD.remove((String("merge")+String(i)+".db").c_str());
		SD.remove((String("merge")+String(i+1)+".db").c_str());
	}

	fileToRename.open((String("merge")+String(mergeCount-1)+".db").c_str(), O_WRITE | O_CREAT);
	fileToRename.rename(SD.vwd(),(database.substring(0,database.indexOf('.')) + "sorted.db").c_str());

	return true;
}

//reads a subset of database determined by memory capacity into a DoublyLinkedList,
//sorts it, and exports it into a temp file
//returns the count of temporary files
uint16_t DatabaseHandler::splitDatabase(String &database, const char delim)
{
	ifstream fin(database.c_str());
	ofstream fout;
	uint16_t tempFileCount = 0;
	uint16_t charsRead = 0;
	String tempFileName = String("temp") + String(tempFileCount++) + ".db";
	String lineRead = "";
	DoublyLinkedList<String> lines;

	//because getline is evaluated first, eof is set before it's checked
	for (char buffer[LINE_BUFFER_SIZE]; fin.getline(buffer,LINE_BUFFER_SIZE,'\n') || !fin.eof();)
	{
		lineRead += buffer;
		charsRead += fin.gcount();

		//if \n was not found by getline
		if(fin.fail())
		{
			fin.clear();	
		}
		else
		{
			lines.add(lineRead);
			lineRead = "";
			if(charsRead > CHARS_ALLOWED_IN_MEMORY || fin.eof())
			{
				fout.open(tempFileName.c_str());
				lines.sort();

				for (DoublyLinkedList<String>::iterator iter = lines.begin();
					!iter.isNull(); ++iter)
				{
					//store iter in the current temporary database file
					// fout << *iter << endl;
				}

				fout.close();
				tempFileName = String("temp") + String(tempFileCount++) + ".db";
				lines.clearList();
				charsRead = 0;
			}
		}
	}
	fin.close();
	return tempFileCount-1;
}

//merges two files
//returns a reference to the mergeCount for use by larger programs can update it
//so that mergeFiles will not overwrite externally created merge files
//note that mergeCount is equal to the number in the current merge file upon function start
//and is off by 1 upon function completion
uint16_t& DatabaseHandler::mergeFiles(const String &f1, const String &f2, const char delim, const String mergeFileName)
{
	static uint16_t mergeCount = 0;
	ifstream file1(f1.c_str()), file2(f2.c_str());
	ofstream mergedFile(mergeFileName.c_str());
	char buffer1[LINE_BUFFER_SIZE], buffer2[LINE_BUFFER_SIZE];
	String line1 = "", line2 = "";

	while(file1.good() && file2.good())
	{
		if(line1 == "")
		{
			file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');

			if(file1.eof())
			{
				break;
			}
			
			while(file1.fail() && !file1.eof())
			{
				line1 += buffer1;
				file1.clear();
				file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
			}
			line1 += buffer1;
		}

		if(line2 == "")
		{
			file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			
			if(file2.eof())
			{
				break;
			}

			while(file2.fail())
			{
				line2 += buffer2;
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
			line2 += buffer2;
		}

		if(line1 > line2)
		{
			// mergedFile << line2 << endl;
			line2 = "";
		}
		else
		{
			// mergedFile << line1 << endl;
			line1 = "";
		}
	}

	if(file1.good())
	{
		// mergedFile << line1 << endl;
		while(file1.getline(buffer1,LINE_BUFFER_SIZE,'\n'))
		{
			line1 = buffer1;
			while(file1.fail() && !file1.eof())
			{
				line1 += buffer1;
				file1.clear();
				file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
			}
			// mergedFile << line1 << endl;
		}
	}
	else if(file2.good())
	{
		// mergedFile << line2 << endl;
		while(file2.getline(buffer2,LINE_BUFFER_SIZE,'\n'))
		{
			line2 = buffer2;
			while(file2.fail())
			{
				line2 += buffer2;
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
			// mergedFile << line2 << endl;
		}

	}

	file1.close();
	file2.close();
	mergedFile.close();

	return mergeCount;
}

// bool createIndexTable(SdFile &database, SdFile &indexfile)
// {
// 	//LOOP: loop through database using ifstream
// 	//REFERENCE: tellg and seekg in ifstream get and seek to locations in files
// 	String field = line.substringing(0,line.firstIndexOf('\t'));
// 	indexfile.println(field + '\t' + ifstream::tellg());
// }

#endif // DATABASEHANDLER_H
