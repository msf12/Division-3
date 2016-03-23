#include "DatabaseCodeTesting2.h"

using std::string; using std::to_string; using std::cout; using std::endl; using std::ifstream; using std::ofstream;

//splits the database into temp files and then merges the files in pairs
//takes optional arguments for primary field delimiter and secondary field list separators
bool sortDatabase(string database, const char delim, const char conn)
{
	const int tempFileCount = splitDatabase(database,delim,conn);
	// int mergeFileCount = 0;
	// for (int i = 0; i < tempFileCount+mergeFileCount; i+=2)
	// {
	// 	mergeFiles(string("temp")+to_string(i)+".db",string("temp")+to_string(i+1)+".db",delim,conn);
	// }
	return true;
}

//reads a subset of database determined by memory capacity into a DoublyLinkedList
//subsequently sorts the list, iterates through it to merge entries with duplicate primary fields separated by delim
//and concatenate secondary fields using conn as the separator
//and exports it into a temp file
//returns the count of temporary files + 1
int splitDatabase(string &database, const char delim, const char conn)
{
	ifstream fin(database);
	ofstream fout;
	int tempFileCount = 0;
	for (char buffer[LINE_BUFFER_SIZE]; fin.getline(buffer,LINE_BUFFER_SIZE,'\n');)
	{
		static int charsRead;
		static string tempFileName = string("temp") + to_string(tempFileCount++) + ".db";
		static string lineRead = "";
		static DoublyLinkedList<string> lines;

		lineRead += buffer;
		charsRead += fin.gcount();

		cout << lineRead << endl;

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

				for (DoublyLinkedList<string>::iterator iter = lines.begin(), jter = iter.next();
					!iter.isNull(); iter = jter, jter = iter.next())
				{
					//if jter is not a null node
					//and the first field of iter (iter[0] -> iter.find(delim)) is equal to the first field of jter
					//append the second field of jter to the second field of iter separated by conn
					//repeat until the first fields differ or jter goes out of the bounds of the list
					while(!jter.isNull() && (*iter).substr(0,(*iter).find(delim)) == (*jter).substr(0,(*jter).find(delim)))
					{
						*iter += conn + (*jter).substr((*jter).find(delim)+1);
						++jter;
					}
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
	return tempFileCount;
}

//merges two files
//eliminates duplicate entries by comparing substrings up to the first occurance of delim
//then combines the lines after delim by concatenating with conn
bool mergeFiles(const string &file1, const string &file2, const char delim, const char conn)
{
	static int mergeCount = 0;
	string mergeFile = string("merge") + to_string(mergeCount++) + ".db";
	return true;
}

int main(int argc, char const *argv[])
{
	sortDatabase(string("test.db"));
	return 0;
}