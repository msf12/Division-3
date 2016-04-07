#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

extern SdFat SD;

class DatabaseHandler
{
	//TODO: load the database in blocks of size DATABASE_BLOCK_SIZE (not sure yet if DoublyLinkedList or String[])
	//prevBlock, currBlock, nextBlock
	//When a user moves significatly out of currBlock, replace currBlock with either:
	//(if they moved into prevBlock)
	//make currBlock point to prevBlock
	//make nextBlock point to currBlock
	//read the database file from the top and load the block before prevBlock into prevBlock
	//(if they moved into nextBlock)
	//make currBlock point to nextBlock
	//make prevBlock point to currBlock
	//read the database file from the current location and load the following block into nextBlock
	//main OS treats the database as if it is entirely in memory
	//so the current index as the OS would store is is converted to index%200 in whichever of the three blocks the user is currently in

	bool populateInitDatabase(const String path = "");
	bool populateInitDatabase(const String path, ofstream &db);

	//splits the database into temp files and then merges the files in pairs
	//takes optional arguments for name of the target file and the field delimiter
	bool sortDatabase(const String &database, const String &target, const char delim = '\t');

	//reads a subset of database determined by memory capacity into a DoublyLinkedList
	//subsequently sorts the list, iterates through it to merge entries with duplicate primary fields separated by delim
	//and concatenate secondary fields using conn as the separator
	//and exports it into a temp file
	//returns the count of temporary files
	uint16_t splitDatabase(const String &database, const char delim = '\t');

	//merges two files
	bool mergeFiles(const String &f1, const String &f2, const char delim = '\t', const String mergeFileName = "merge.db");

	uint16_t generateAssociations(const String &database, const char delim = '\t');

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
	
	uint8_t setup()
	{
		if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
		{
			return 2;
		}

		if(SD.exists("sorted.db"))
		{
			// return 0;
		}

		// if(!SD.exists("init.db"))
		// {
			populateInitDatabase(String("/Music"));
		// }
		sortDatabase("init.db","sorted.db");
		uint8_t assocFileCount = generateAssociations("sorted.db");
		
		// assoc0.db is already sorted when init is sorted
		// for(int i = 1; i < assocFileCount; ++i)
		// {
		// 	sortDatabase("assoc" + String(i) + ".db", "assoc" + String(i) + "sorted.db");
		// }
	
		return 1;
	}

};

bool DatabaseHandler::populateInitDatabase(const String path)
{
	ofstream initdb("init.db");
	populateInitDatabase(path,initdb);
	initdb.close();
	return true;
}

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
bool DatabaseHandler::populateInitDatabase(const String path, ofstream &db)
{
	char fileName[250];
	FatFile directory(path.c_str(), O_READ);
	String artist = "",
		album = "",
		song = "";

	uint8_t split1,split2;

	for(SdFile file; file.openNext(&directory,O_READ);)
	{
		file.getName(fileName,250);

		if(!file.isDir())
		{
			String extension = fileName;
			extension = extension.substring(extension.lastIndexOf('.'));
			if(extension.equalsIgnoreCase(".mp3") ||
				extension.equalsIgnoreCase(".flac") ||
				extension.equalsIgnoreCase(".aac"))
			{
				split1 = path.indexOf('/',1);
				split2 = path.indexOf('/',split1+1);
				artist = path.substring(split1+1,split2);
				album = path.substring(split2+1);

				song = fileName;
				split1 = song.lastIndexOf('-');
				split2 = song.lastIndexOf('-',split1-1);

				song = song.substring(split1+1,song.lastIndexOf('.')) + '-' + song.substring(0,split2);
				// Serial.println(artist + '\t' + album + '\t' + song);
				
				db << artist.c_str() << '\t' << album.c_str() << '\t' << song.c_str() << '\t' << (path + '/' + fileName).c_str() << '\n';
			}

		}
		else
		{
			populateInitDatabase(path + "/" + fileName,db);
		}
		file.close();
	}
	db.flush();
	directory.close();
	return true;
}

bool DatabaseHandler::sortDatabase(const String &database, const String &target, const char delim)
{
	Serial.println("Sorting " + database + " into " + target);

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
		SD.remove((String("merge")+String(mergeCount)+".db").c_str());

		fileToRename.open((String("temp")+String(tempFileCount-1)+".db").c_str(), O_READ | O_WRITE | O_CREAT);
		fileToRename.rename(SD.vwd(),(String("merge")+String(mergeCount++)+".db").c_str());
		fileToRename.close();
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

	SD.remove(target.c_str());
	fileToRename.open((String("merge")+String(mergeCount-1)+".db").c_str(), O_READ | O_WRITE | O_CREAT);
	fileToRename.rename(SD.vwd(),target.c_str());
	fileToRename.close();

	return true;
}

//reads a subset of database determined by memory capacity into a DoublyLinkedList,
//sorts it, and exports it into a temp file
//returns the count of temporary files
uint16_t DatabaseHandler::splitDatabase(const String &database, const char delim)
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
					fout << (*iter).c_str() << '\n';
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
//so that mergeFiles will not overwrite externally created merge files
//and is off by 1 upon function completion
bool DatabaseHandler::mergeFiles(const String &f1, const String &f2, const char delim, const String mergeFileName)
{
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

			while(file2.fail() && !file2.eof())
			{
				line2 += buffer2;
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
			line2 += buffer2;
		}

		if(line1 > line2)
		{
			mergedFile << line2.c_str() << '\n';
			line2 = "";
		}
		else
		{
			mergedFile << line1.c_str() << '\n';
			
			//if they are the same ensure the database will not have duplicate lines
			if(line1 == line2)
			{
				line2 = "";
			}

			line1 = "";
		}
	}


	if(file1.good())
	{
		if(line1 != "")
		{
			mergedFile << line1.c_str() << '\n';
		}
		while(!file1.eof())
		{
			file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
			line1 = buffer1;
			while(file1.fail() && !file1.eof())
			{
				file1.clear();
				file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
				line1 += buffer1;
			}
			mergedFile << line1.c_str() << '\n';
			line1 = "";
		}
	}
	else if(file2.good())
	{
		// Serial.println(line2);
		if(line2 != "")
		{
			mergedFile << line2.c_str() << '\n';
		}
		while(!file2.eof())
		{
			file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			line2 = buffer2;
			while(file2.fail() && !file2.eof())
			{
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
				line2 += buffer2;
			}
			// Serial.println(line2);
			mergedFile << line2.c_str() << '\n';
		}

	}

	file1.close();
	file2.close();
	mergedFile.close();

	return true;
}

uint16_t DatabaseHandler::generateAssociations(const String &database, const char delim)
{
	Serial.println("Generating associations from " + database);

	ifstream db(database.c_str());
	char buffer[LINE_BUFFER_SIZE];
	String line = "";
	do
	{
		db.clear();
		db.getline(buffer,LINE_BUFFER_SIZE,'\n');
		line += buffer;
	} while(db.fail());

	uint8_t numFields = 1; //number of fields in the sorted database

	for(int i = 0; i < line.length(); ++i)
	{
		if(line[i] == delim)
		{
			++numFields;
		}
	}
	db.seekg(0);

	Serial.println(numFields);

	ofstream assocFiles[numFields]; //one association file per field

	for(int i = 0; i < numFields-1; ++i)
	{
		Serial.println(String("Opening file: ") + "assoc" + String(i) + ".db");
		SD.remove((String("Opening file: ") + "assoc" + String(i) + ".db").c_str());
		assocFiles[i].open(("assoc" + String(i) + ".db").c_str());
	}
	Serial.println("test");
	
	uint16_t IDcount[numFields-2], //numFields-1 pairs and the final one doesn't need an ID
		delimLocs[numFields-1]; //there are numFields-1 delimiters in each line
	String fields[numFields], //array to store the field values
		prevFields[numFields]; //previous field values for comparison

	for(int i = 0; i < numFields; ++i)
	{
		if(i < numFields-1)
		{
			IDcount[i] = 0;
		}
		prevFields[i] = "";
	}

	line = "";
	while(db.getline(buffer, LINE_BUFFER_SIZE, '\n') || !db.eof())
	{
		line += buffer;
		if(db.fail())
		{
			db.clear();
		}
		else
		{
			//get first field before the loop because it requires a set first value
			delimLocs[0] = line.indexOf(delim);
			fields[0] = line.substring(0,delimLocs[0]);	
			for(int i = 1; i < numFields-1; ++i)
			{
				delimLocs[i] = line.indexOf(delim,delimLocs[i-1]+1);
				fields[i] = line.substring(delimLocs[i-1]+1,delimLocs[i]);
			}
			//get last field after the loop because there is no second delimiter
			fields[numFields-1] = line.substring(delimLocs[numFields-2]+1);

			//assocFiles[0] << fields[0] << '\t' << fields[1] << '\t' << IDcount[0] << '\n'
			//assocFiles[1] << fields[1] << '\t' << IDcount[0]++ << '\t' << fields[2] << '\t' << IDcount[1] << '\n'
			//assocFiles[2] << fields[2] << '\t' << IDcount[1]++ << '\t' << fields[3] << '\n'

			//if field 0 or field 1 have changed there is a new association to add to assoc[0]
			if(fields[0] != prevFields[0] || fields[1] != prevFields[1])
			{
				++IDcount[0];
				assocFiles[0] << fields[0].c_str() << '\t' << fields[1].c_str() << '\t' << String(IDcount[0]).c_str() << '\n';
				prevFields[0] = fields[0];
			}

			for(int i = 1; i < numFields-2; ++i)
			{
				//if fields i or field i+1 have changed there is a new association to add to assoc[0]
				if(fields[i] != prevFields[i] || fields[i+1] != prevFields[i+1])
				{
					++IDcount[i];
					assocFiles[i] << fields[i].c_str() << '\t' << String(IDcount[i-1]).c_str() << '\t' <<
						fields[i+1].c_str() << '\t' << String(IDcount[i]).c_str() << '\n';
					prevFields[i] = fields[i];
					prevFields[i+1] = fields[i+1];
				}
			}
			
			//numFields-1 association files so the final one is at index numFields-2

			assocFiles[numFields-2] << fields[numFields-2].c_str() << '\t' << String(IDcount[numFields-3]).c_str() << '\t' <<
				fields[numFields-1].c_str() << '\n';

			line = "";
		}
	}

	for(int i = 0; i < numFields-1; ++i)
	{
		assocFiles[i].close();
	}

	return numFields-1;
}

// bool createIndexTable(SdFile &database, SdFile &indexfile)
// {
// 	//LOOP: loop through database using ifstream
// 	//REFERENCE: tellg and seekg in ifstream get and seek to locations in files
// 	String field = line.substringing(0,line.firstIndexOf('\t'));
// 	indexfile.println(field + '\t' + ifstream::tellg());
// }

#endif // DATABASEHANDLER_H
