#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include "DoublyLinkedList.h"

using std::string; using std::cout; using std::endl; using std::ifstream; using std::ofstream;

bool sortDatabase(string &database)
{
	const int line_buffer_size = 50;
	char buffer[line_buffer_size];

	string line = ""; //string to hold the appended contents of the buffers for the current line

	ifstream sdin(database); //ifstream to read from the file named in the database string
	
	int tempFileCount = 0; //number for procedural generation of temporary files

	DoublyLinkedList<string> lines; //list of the lines that were read
	
	//procedurally generated file name for temporary files
	string tempFileName = "temp";
	tempFileName += std::to_string(tempFileCount);
	tempFileName += ".db";

	ofstream tempFile(tempFileName); //ofstream to output to the temporary files
	
	//ifstream gets a line until a delimiter and stores it in a char[] buffer
	for(int charsRead = 0; sdin.getline(buffer, line_buffer_size, '\n'); charsRead+=sdin.gcount()) //while getline succeeds
	{
		line += buffer; //add the new buffer to the total contents of the line
		if (sdin.fail()) //fin's failbit is set to 1 if the delimiter '\n' was not found
		{
			sdin.clear(); //clear the state bits
		}
		else //if getline did not set the failbit add the current line to the list and clear the line variable
		{
			// cout << line << endl;
			lines.add(line);
			line = "";
		}
		//once the number of characters read has reached the max allowable in memory
		//sort and store the information, removing duplicates by appending child fields into comma sorted lists
		if(charsRead > 5000)
		{
			lines.sort();
			//for iterators i and j where i starts at index 0 of the list and j starts at index 1
			//while i is not the end of the list
			//set i equal to j and increment j
			for(DoublyLinkedList<string>::iterator i = lines.begin(), j = i.next(); i != lines.end(); i = j, ++j)
			{
				//if j is not a null node
				//and the first field of i (i[0] -> i.find('\t')) is equal to the first field of j
				//append the second field of j to the second field of i separated by a comma
				//repeat until the first fields differ or j goes out of the bounds of the list
				while(!j.isNull() && (*i).substr(0,(*i).find('\t')) == (*j).substr(0,(*j).find('\t')))
				{
					*i += ",";
					*i += (*j).substr((*j).find('\t')+1);
					++j;
				}
				//store i in the current temporary database file
				tempFile << *i << endl;

				//if j has gone outside the bounds of the list, end the loop
				if(j.isNull())
				{
					break;
				}
			}

			//close the current temporary file
			tempFile.close();
			
			//change to the next temporary file, open it, and clear lines
			// cout << "Changing from " << tempFileName << " to ";
			tempFileName = "temp";
			tempFileName += std::to_string(++tempFileCount);
			tempFileName += ".db";
			// cout << tempFileName << endl;
			
			tempFile.open(tempFileName);
			charsRead = 0;
			lines.clearList();
		}
	}

	//the final contents of lines are not printed by the loop so print them here
	lines.sort();
	for(DoublyLinkedList<string>::iterator i = lines.begin(), j = i.next(); i != lines.end(); i = j, ++j)
	{
		while(!j.isNull() && (*i).substr(0,(*i).find('\t')) == (*j).substr(0,(*j).find('\t')))
		{
			*i += ",";
			*i += (*j).substr((*j).find('\t')+1);
			++j;
		}
		tempFile << *i << endl;
		if(j.isNull())
		{
			break;
		}
	}
	tempFile.close();
	lines.clearList();
	
//LOOP: loop through tempFiles in pairs and merge them, consolidating duplicates each time

	for(int currFileNum = 0; currFileNum < tempFileCount; currFileNum+=2)
	{
		static string currFileName = "";
		currFileName = "temp";
		currFileName += std::to_string(currFileNum);
		currFileName += ".db";
		line = "";
		static ifstream currFile;
		currFile.open(currFileName);

		static char buffer2[line_buffer_size];
		static string currFile2Name = "";
		currFile2Name = "temp";
		currFile2Name += std::to_string(currFileNum+1);
		currFile2Name += ".db";
		static string line2;
		line2 = "";
		static ifstream currFile2;
		currFile2.open(currFile2Name);

		tempFileName = "temp";
		tempFileName += std::to_string(++tempFileCount);
		tempFileName += ".db";
		tempFile.open(tempFileName);

		cout << "Merging\ncurrFile:\t" << currFileName << "\ncurrFile2:\t" << currFile2Name << "tempFile:\t" << tempFileName << endl;

		//while both filestreams have no error bits
		while(currFile.good() && currFile2.good())
		{
			//if either line string is empty
			if(line == "")
			{
				//get the new buffer
				currFile.getline(buffer, line_buffer_size, '\n');
				
				//while there is data left to read (the getline set the fail bit but not the eof one) append it to the line variable
				while(currFile.fail() && !currFile.eof())
				{
					currFile.clear();
					line += buffer;
					currFile.getline(buffer, line_buffer_size, '\n');
				}
				line += buffer;
			}
			if(line2 == "")
			{
				currFile2.getline(buffer2, line_buffer_size, '\n');
				while(currFile2.fail() && !currFile2.eof())
				{
					currFile2.clear();
					line2 += buffer2;
					currFile2.getline(buffer2, line_buffer_size, '\n');
				}
				line2 += buffer2;
			}

			//compare the first field of line and line2 and combine or order them appropriately
			// cout << "comparing " << line << " and " << line2 << endl;

			if(line == "" || line2 == "")
			{
				break;
			}

			if(line.substr(0,line.find('\t')) < line2.substr(0,line2.find('\t')))
			{
				tempFile << line << endl;
				line = "";
			}
			else if(line.substr(0,line.find('\t')) > line2.substr(0,line2.find('\t')))
			{
				tempFile << line2 << endl;
				line2 = "";
			}
			else
			{
				tempFile << line << "," << line2.substr(line2.find('\t')+1) << endl;
				line = "";
				line2 = "";
			}
		}

		//if a file has remaining information, add it to the end of the new file
		if(currFile.good() && !currFile2.good())
		{
			tempFile << line << endl;
			line = "";
			while(currFile.getline(buffer, line_buffer_size, '\n'))
			{
				line += buffer;
				if(currFile.fail())
				{
					currFile.clear();
				}
				else
				{
					tempFile << line << endl;
					line = "";
				}
			}
		}
		else if(currFile2.good() && !currFile.good())
		{
			tempFile << line2 << endl;
			line2 = "";
			while(currFile2.getline(buffer2, line_buffer_size, '\n'))
			{
				line2 += buffer2;
				if(currFile2.fail())
				{
					currFile2.clear();
				}
				else
				{
					line2 = "";
					tempFile << line2 << endl;
				}
			}
		}
		currFile.close();
		currFile2.close();
		tempFile.close();
	}

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