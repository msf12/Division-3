#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include "DoublyLinkedList.h"

using std::string; using std::cout; using std::endl; using std::ifstream; using std::ofstream;

bool sortDatabase(string &database)
{
	//PSEUDOCODE

	// int tempCount = 0; //count of temporary database files created from the contents of database
	// ifstream fin(database); //input stream for reading from database
	// char buffer[50];

	// while(fin.getline(buffer,50,'\n') || fin.gcount())
	// {
	// 	static string currLine;
	// 	//ifstream reads to a buffer and sets the failbit to 1 if it doesn't find the passed delimiter
	// 	while(fin.fail())
	// 	{
	// 		currLine += buffer;
	// 	}
	// 	currLine += buffer;
	// 	// cout << currLine << endl;
	// }

	const int line_buffer_size = 50;
	char buffer[line_buffer_size];
	string line = "";
	ifstream sdin("test.db");
	int line_number = 0,
		currentTempFile = 0;
	string tempFileName = "temp";
	tempFileName += std::to_string(currentTempFile);
	tempFileName += ".db";
	ofstream tempFile(tempFileName);
	
	//ifstream gets a line until a delimiter and stores it in a char[] buffer
	for(int charsRead = 0; sdin.getline(buffer, line_buffer_size, '\n'); charsRead+=sdin.gcount()) //while getline succeeds
	{
		line += buffer; //add the new buffer to the total contents of the line
		if (sdin.fail()) //fin's failbit is set to 1 if the delimiter '\n' was not found
		{
			sdin.clear(); //clear the state bits
		}
		else //if getline did not set the failbit print the current line and clear the line variable
		{
			tempFile << line << endl;
			cout << line << endl;
			line = "";
		}
		if(charsRead > 5000)
		{
			tempFile.close();
			cout << "Changing from " << tempFileName << " to ";
			tempFileName = "temp";
			tempFileName += std::to_string(++currentTempFile);
			tempFileName += ".db";
			cout << tempFileName << endl;
			tempFile.open(tempFileName);
			charsRead = 0;
		}
	}

	//LOOP: for the entirety of database
// 	while(fin.getline())
// 	{
// 		//Struct is a placeholder name for a struct holding a single term and a single associated piece of information (EX. album - song)
// 		//list is a placeholder name for the list of Structs
// 		//mem is an uint16_t representing the amount of Structs that can be simultaneously held in memory
// 		DoublyLinkedList<Struct> list();

// 		for(uint16_t i = 0; i < mem; ++i)
// 		{
// 			Struct *s = new Struct();
// 			String info = buffer;
// 			s->field1 = info.substring(0,info.firstIndexOf("\t"));
// 			s->field2 = info.substring(info.firstIndexOf("\t")+1); //make sure not to include '\n'
// 			list.add(*s);
// 		}
// 		list.sort();

// 		//create a new temporary file with a default name (EX. tempartists3.db)
// 		SdFile tempFile("defaultname" + tempCount + ".db");
		
// 		for(DoublyLinkedList<Struct>::iterator i = list.begin(); i != list.end(); ++i)
// 		{
// 			//add the first and second fields of the Struct pointed at by i to the database file
// 			tempFile.print((*i)->field1 + "\t" + field2);

// 			//while the first field of the Struct pointed to by i is equal to
// 			//the first field of the Struct pointed at by i.next()
// 			//add the second field of i.next() to the current line of the sorted database
// 			while((*i)->field1 == (*(i.next()))->field1)
// 			{
// 				++i;
// 				tempFile.print("," + (*i)->);
// 			}
// 			tempFile.println();
// 		}
// 	}
	
// 	//ENDLOOP: loop through tempFiles in pairs and merge them, consolidating duplicates each time

// 	while(tempCount > 2)
// 	{
// 		uint8_t numPairs = tempCount/2; //number of temporary file pairs possible
// 		for(int i = 0; i < numPairs; ++i)
// 		{
// 			SdFile file1("defaultname" + i + ".db");
// 			SdFile file2("defaultname" + ++i + ".db");
// 			mergeFiles(file1,file2);
// 			tempCount-1;
// 		}
// 	}
// 	//manually merge into new artists.db file
}

// bool createIndexTable(SdFile &database, SdFile &indexfile)
// {
// 	//LOOP: loop through database using ifstream
// 	//REFERENCE: tellg and seekg in ifstream get and seek to locations in files
// 	String field = line.substring(0,line.firstIndexOf('\t'));
// 	indexfile.println(field + '\t' + ifstream::tellg());
// }

int main(int argc, char const *argv[])
{
	string s = "test.db";
	sortDatabase(s);	
	return 0;
}