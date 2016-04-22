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
		//attempt to connect to the SD card
		//if the attempt fails return error code 2
		if(!SD.begin(SD_CS_PIN, SPI_HALF_SPEED))
		{
			return 2;
		}

		//check for the existence of the database files
		//return with no error if they already exist
		//the names can be translated to:
		//artist album; album songnumber; songnumber song; song path;
		if(SD.exists("aral.db") && SD.exists("alsn.db") && SD.exists("snsg.db") && SD.exists("sgpa.db"))
		{
			return 0;
		}

		//does the init database exist
		if(!SD.exists("init.db"))
		{
			//if not, create it and remove the final databases for recreation
			populateInitDatabase(String("/Music"));
			SD.remove("sorted.db");
			SD.remove("aral.db");
			SD.remove("alsn.db");
			SD.remove("snsg.db");
			SD.remove("sgpa.db");
		}

		//does the sorted init database exist
		if(!SD.exists("sorted.db"))
		{
			sortDatabase("init.db","sorted.db");
		}

		//generate association files and store the count of them
		uint8_t assocFileCount = generateAssociations("sorted.db");
		
		//sort the association files
		//assoc0.db is already sorted when init is sorted
		for(int i = 1; i < assocFileCount; ++i)
		{
			sortDatabase("assoc" + String(i) + ".db", "assoc" + String(i) + "sorted.db");
			SD.remove(("assoc" + String(i) + ".db").c_str());
		}

		//rename the association files into the final filenames
		//this is hardcoded but could potentially be automated
		SdFile file;
		file.open("assoc0.db");
		file.rename(SD.vwd(),"aral.db");
		file.close();
		file.open("assoc1sorted.db");
		file.rename(SD.vwd(),"alsn.db");
		file.close();
		file.open("assoc2sorted.db");
		file.rename(SD.vwd(),"snsg.db");
		file.close();
		file.open("assoc3sorted.db");
		file.rename(SD.vwd(),"sgpa.db");
		file.close();
	
		return 1;
	}

};

bool DatabaseHandler::populateInitDatabase(const String path)
{
	ofstream initdb("init.db");
	//Field titles start with " # " as ' ' and '#' are two of the earliest ASCII values
	//and that combination cannot reasonably be expected to appear at the beginning of any field value
	initdb << " # Artist	 # Album	 # Song Number	 # Song	 # Path\n";
	populateInitDatabase(path,initdb);
	initdb.close();
	return true;
}

//SD.vwd() returns an SdBaseFile
//Thus I can iterate through subdirectories recursively using the directory file as a parameter
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

//populateInitDatabase currently relies on a predefined directory structure
//the finished design should instead use file tags instead
bool DatabaseHandler::populateInitDatabase(const String path, ofstream &db)
{
	char fileName[250]; //250 is the maximum allowed length of a long file name according to the FAT32 standard
	FatFile directory(path.c_str(), O_READ); //create a FatFile instance for the current directory for iteration
	String artist = "",
		album = "",
		song = "",
		songNumber = ""; //placement in album

	uint8_t split1,split2; //ints used for indexes of special characters in the path

	for(SdFile file; file.openNext(&directory,O_READ);)
	{
		file.getName(fileName,250);

		//if the current file in the directory is not a subdirectory
		if(!file.isDir())
		{
			//get the filename extension to insure it's a music file
			String extension = fileName;
			extension = extension.substring(extension.lastIndexOf('.'));
			if(extension.equalsIgnoreCase(".mp3") ||
				extension.equalsIgnoreCase(".flac") ||
				extension.equalsIgnoreCase(".aac"))
			{
				//if it's a music file, the path contains the information in the form
				// "/artist/album/songname-discnumber-songnumber.extension"
				split1 = path.indexOf('/',1);
				split2 = path.indexOf('/',split1+1);
				artist = path.substring(split1+1,split2); //artist is between the first pair of '/' characters
				album = path.substring(split2+1); //artist is between the second pair of '/' characters

				song = fileName;
				split1 = song.lastIndexOf('-');
				split2 = song.lastIndexOf('-',split1-1);

				songNumber = song.substring(split1+1,song.lastIndexOf('.')); //songNumber is between the final '-' and the '.'

				//pad the songNumber string with 0's to more easily sort the strings
				while(songNumber.length() < 3)
				{
					songNumber = '0' + songNumber;
				}

				//song name is the filename up to the first '-'
				song = song.substring(0,split2);
				
				//print all the fields to the database separated by tabs
				db << artist.c_str() << '\t' << album.c_str() << '\t' <<
					songNumber.c_str() << '\t' << song.c_str() << '\t' << (path + '/' + fileName).c_str() << '\n';
			}

		}
		//else recursively populate the init database
		else
		{
			populateInitDatabase(path + "/" + fileName,db);
		}
		//close the file to open the next one
		file.close();
	}
	//print changes to the database from the buffer
	db.flush();
	//close the directory and return
	directory.close();
	return true;
}

bool DatabaseHandler::sortDatabase(const String &database, const String &target, const char delim)
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

//merges two files into a third file of a given name
bool DatabaseHandler::mergeFiles(const String &f1, const String &f2, const char delim, const String mergeFileName)
{
	ifstream file1(f1.c_str()), file2(f2.c_str());
	ofstream mergedFile(mergeFileName.c_str());
	char buffer1[LINE_BUFFER_SIZE], buffer2[LINE_BUFFER_SIZE];
	String line1 = "", line2 = "";

	//while both files have not reached an eof
	while(file1.good() && file2.good())
	{
		//ensure both line1 and line2 contain a full line
		if(line1 == "")
		{
			file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');

			if(file1.eof())
			{
				break;
			}
			
			//while the getline has not yet found a '\n' (line1 is not a complete line)
			while(file1.fail() && !file1.eof())
			{
				line1 += buffer1;
				file1.clear(); //clear the fail bit
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

			//while the getline has not yet found a '\n' (line2 is not a complete line)
			while(file2.fail() && !file2.eof())
			{
				line2 += buffer2;
				file2.clear(); //clear the fail bit
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
			line2 += buffer2;
		}

		//ignore empty lines
		if(line1 == "\n")
		{
			line1 = "";
			continue;
		}
		else if(line2 == "\n")
		{
			line2 = "";
			continue;
		}

		//compare the two lines and print the lexicographically lesser line
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

	//if either file is still good after one has reached an eof
	//loop through the remainder of that file printing nonempty lines to the merged file
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
			if(line1 == "\n" || line1 == "")
			{
				line1 = "";
				continue;
			}
			mergedFile << line1.c_str() << '\n';
			line1 = "";
		}
	}
	else if(file2.good())
	{
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
			if(line2 == "\n" || line2 == "")
			{
				line2 = "";
				continue;
			}
			mergedFile << line2.c_str() << '\n';
		}

	}

	file1.close();
	file2.close();
	mergedFile.close();

	return true;
}

//generate association files from fields of a database file
uint16_t DatabaseHandler::generateAssociations(const String &database, const char delim)
{
	ifstream db(database.c_str());
	char buffer[LINE_BUFFER_SIZE];
	String line = "";

	//get the first line and use it to count the number of fields in the database
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
	//set the read to start from the beginning again
	db.seekg(0);

	ofstream assocFiles[numFields]; //one association file per field

	//open the ofstreams
	for(int i = 0; i < numFields-1; ++i)
	{
		assocFiles[i].open(("assoc" + String(i) + ".db").c_str());
	}
	
	uint16_t IDcount[numFields-2], //numFields-1 pairs and the final one doesn't need an ID
		delimLocs[numFields-1]; //there are numFields-1 delimiters in each line
	String fields[numFields], //array to store the field values
		prevFields[numFields], //previous field values for comparison
		IDbuilder; //string to build the final ID number to standardize the length of ID strings

	//initialize IDcounts and prevFields
	for(int i = 0; i < numFields; ++i)
	{
		if(i < numFields-1)
		{
			IDcount[i] = 0;
		}
		prevFields[i] = "";
	}

	line = "";
	//read the file and print the field associations to the association files
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

			//loop through the remaining fields and store them
			for(int i = 1; i < numFields-1; ++i)
			{
				delimLocs[i] = line.indexOf(delim,delimLocs[i-1]+1);
				fields[i] = line.substring(delimLocs[i-1]+1,delimLocs[i]);
			}

			//get last field after the loop because there is no second delimiter
			fields[numFields-1] = line.substring(delimLocs[numFields-2]+1);

			//Simplified example of the intended results of the following code for reference:
			//assocFiles[0] << fields[0] << '\t' << fields[1] << '\t' << IDcount[0] << '\n'
			//assocFiles[1] << fields[1] << '\t' << IDcount[0]++ << '\t' << fields[2] << '\t' << IDcount[1] << '\n'
			//assocFiles[1] << fields[2] << '\t' << IDcount[1]++ << '\t' << fields[3] << '\t' << IDcount[2] << '\n'
			//assocFiles[2] << fields[3] << '\t' << IDcount[2]++ << '\t' << fields[4] << '\n'

			//if field 0 or field 1 have changed there is a new association to add to assoc[0]
			if(fields[0] != prevFields[0] || fields[1] != prevFields[1])
			{
				//increment the association ID
				++IDcount[0];
				
				//convert the ID to a string and pad it with zeros for sorting purposes
				IDbuilder = String(IDcount[0]);
				while(IDbuilder.length() < 7)
				{
					IDbuilder = '0' + IDbuilder;
				}
				assocFiles[0] << fields[0].c_str() << '\t' << fields[1].c_str() << '\t' << IDbuilder.c_str() << '\n';
				prevFields[0] = fields[0];
			}

			for(int i = 1; i < numFields-2; ++i)
			{
				//if fields i or field i+1 have changed there is a new association to add to assoc[0]
				if(fields[i] != prevFields[i] || fields[i+1] != prevFields[i+1])
				{
					//increment the association ID for the current association
					++IDcount[i];

					
					//convert the ID of the previous association to a string and pad it with zeros for sorting purposes
					IDbuilder = String(IDcount[i-1]);
					while(IDbuilder.length() < 7)
					{
						IDbuilder = '0' + IDbuilder;
					}

					//print the current field and it's association ID with the previous field (the previous association ID)
					assocFiles[i] << fields[i].c_str() << '\t' << IDbuilder.c_str() << '\t';

					//convert the ID of the current association to a string and pad it with zeros for sorting purposes
					IDbuilder = String(IDcount[i]);
					while(IDbuilder.length() < 7)
					{
						IDbuilder = '0' + IDbuilder;
					}

					//print the next field and it's association ID with the current field (the current association ID)
					assocFiles[i] << fields[i+1].c_str() << '\t' << IDbuilder.c_str() << '\n';

					prevFields[i] = fields[i];
					prevFields[i+1] = fields[i+1];
				}
			}
			
			//numFields-1 association files so the final one is at index numFields-2

			IDbuilder = String(IDcount[numFields-3]);
			while(IDbuilder.length() < 7)
			{
				IDbuilder = '0' + IDbuilder;
			}
			assocFiles[numFields-2] << fields[numFields-2].c_str() << '\t' << IDbuilder.c_str() << '\t' <<
				fields[numFields-1].c_str() << '\n';

			line = "";
		}
	}

	//close the association files
	for(int i = 0; i < numFields-1; ++i)
	{
		assocFiles[i].close();
	}

	return numFields-1;
}

#endif // DATABASEHANDLER_H
