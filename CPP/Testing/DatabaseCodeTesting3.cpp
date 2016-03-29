#include "DatabaseCodeTesting3.h"

//splits the database into temp files and then merges the files in pairs
//takes optional arguments for primary field delimiter and secondary field list separators
bool sortDatabase(string database, const char delim)
{
	const int tempFileCount = splitDatabase(database,delim);
	int mergeCount = 0;
	for (int i = 0; i < tempFileCount-1; i+=2)
	{
		mergeFiles(string("temp")+to_string(i)+".db",
			string("temp")+to_string(i+1)+".db",
			delim,
			string("merge")+to_string(mergeCount++)+".db");
		std::remove((string("temp")+to_string(i)+".db").c_str());
		std::remove((string("temp")+to_string(i+1)+".db").c_str());
	}

	//if there are an odd number of temporary files
	//rename the final temporary file to a merge file for automated merging later
	//this merge file will always have the name merge[tempFileCount/2].db
	//this results from every pair of temp files being merged (tempFileCount/2 merge files)
	//and file names starting at merge0.db 
	if(tempFileCount%2)
	{
		std::rename((string("temp")+to_string(tempFileCount-1)+".db").c_str(),
			(string("merge")+to_string(mergeCount++)+".db").c_str());
	}

	for (int i = 0; i < mergeCount-1; i+=2)
	{
		mergeFiles(string("merge")+to_string(i)+".db",
			string("merge")+to_string(i+1)+".db",
			delim,
			string("merge")+to_string(mergeCount++)+".db");
		std::remove((string("merge")+to_string(i)+".db").c_str());
		std::remove((string("merge")+to_string(i+1)+".db").c_str());
	}

	std::rename((string("merge")+to_string(mergeCount-1)+".db").c_str(),
		(database.substr(0,database.find('.')) + "sorted.db").c_str());

	return true;
}

//reads a subset of database determined by memory capacity into a DoublyLinkedList,
//sorts it, and exports it into a temp file
//returns the count of temporary files
int splitDatabase(string &database, const char delim)
{
	ifstream fin(database);
	ofstream fout;
	int tempFileCount = 0;
	int charsRead;
	string tempFileName = string("temp") + to_string(tempFileCount++) + ".db";
	string lineRead = "";
	DoublyLinkedList<string> lines;

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
				fout.open(tempFileName);
				lines.sort();

				for (DoublyLinkedList<string>::iterator iter = lines.begin();
					!iter.isNull(); ++iter)
				{
					//store iter in the current temporary database file
					fout << *iter << endl;
				}

				fout.close();
				tempFileName = string("temp") + to_string(tempFileCount++) + ".db";
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
int& mergeFiles(const string &f1, const string &f2, const char delim, const string mergeFileName)
{
	static int mergeCount = 0;
	ifstream file1(f1), file2(f2);
	ofstream mergedFile(mergeFileName);
	char buffer1[LINE_BUFFER_SIZE], buffer2[LINE_BUFFER_SIZE];
	string line1 = "", line2 = "";

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
			mergedFile << line2 << endl;
			line2 = "";
		}
		else
		{
			mergedFile << line1 << endl;
			line1 = "";
		}
	}

	if(file1.good())
	{
		mergedFile << line1 << endl;
		while(file1.getline(buffer1,LINE_BUFFER_SIZE,'\n'))
		{
			line1 = buffer1;
			while(file1.fail() && !file1.eof())
			{
				line1 += buffer1;
				file1.clear();
				file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
			}
			mergedFile << line1 << endl;
		}
	}
	else if(file2.good())
	{
		mergedFile << line2 << endl;
		while(file2.getline(buffer2,LINE_BUFFER_SIZE,'\n'))
		{
			line2 = buffer2;
			while(file2.fail())
			{
				line2 += buffer2;
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
			mergedFile << line2 << endl;
		}

	}

	file1.close();
	file2.close();
	mergedFile.close();

	return mergeCount;
}

int generateIDFiles(const string &database, const char delim)
{
	ifstream fin(database);

	//field0 -> artist
	//field1 -> album
	//field2 -> song	path
	string prevFields[2], currFields[3];

	//artist	artistid
	//album	albumid
	//song	path	songid
	//artistid	albumid
	//albumid	songid
	ofstream tables[5];
	
	//fieldID[0] -> artistid
	//fieldID[1] -> albumid
	//fieldID[2] -> songid
	int fieldID[3] = {-1,-1,-1}, delim1Index, delim2Index;

	for(int i = 0; i < 5; ++i)
	{
		tables[i].open(string("table") + to_string(i) + ".db");
	}

	for(char buffer[LINE_BUFFER_SIZE]; fin.getline(buffer,LINE_BUFFER_SIZE,'\n') || !fin.eof();)
	{
		static string lineRead = "", prevArtist, prevAlbum;

		lineRead += buffer;

		if(fin.fail())
		{
			fin.clear();
		}
		else
		{
			delim1Index = lineRead.find(delim);
			delim2Index = lineRead.find(delim,delim1Index+1);
			currFields[0] = lineRead.substr(0,delim1Index);
			currFields[1] = lineRead.substr(delim1Index+1, delim2Index-(delim1Index+1));
			currFields[2] = lineRead.substr(delim2Index+1);

			if(prevFields[0] != currFields[0])
			{
				tables[0] << currFields[0] << '\t' << ++fieldID[0] << '\n';
				prevFields[0] = currFields[0];
			}
			if(prevFields[1] != currFields[1])
			{
				tables[1] << currFields[1] << '\t' << ++fieldID[1] << '\n';
				tables[3] << fieldID[0] << '\t' << fieldID[1] << '\n';
				prevFields[1] = currFields[1];
			}

			tables[2] << currFields[2] << '\t' << ++fieldID[2] << '\n';
			tables[4] << fieldID[1] << '\t' << fieldID[2] << '\n';

			lineRead = "";
		}
	}

	fin.close();

	return 3; //return count of ID definition files 
}

int main(int argc, char const *argv[])
{
	// sortDatabase(string("test.db"));
	int numIDTables = generateIDFiles(string("testsorted.db"));
	
	//artist table is already sorted due to the order of the database after the initial sort
	for(int i = 1; i < numIDTables; ++i)
	{
		sortDatabase(string("table") + to_string(i) + ".db");
	}

	return 0;
}