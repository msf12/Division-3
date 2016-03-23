#include "DatabaseCodeTesting2.h"

using std::string; using std::to_string; using std::cout; using std::endl; using std::ifstream; using std::ofstream;

//splits the database into temp files and then merges the files in pairs
//takes optional arguments for primary field delimiter and secondary field list separators
bool sortDatabase(string database, const char delim, const char conn)
{
	const int tempFileCount = splitDatabase(database,delim,conn);
	int mergeFileCount = 0;
	for (int i = 0; i < tempFileCount-1; i+=2)
	{
		mergeFiles(string("temp")+to_string(i)+".db",string("temp")+to_string(i+1)+".db",delim,conn);
	}

	//if there are an odd number of temporary files
	//merge the remaining file with the final merged file
	//this merge file will always have the name merge[tempFileCount/2-1].db
	//this results from every pair of temp files being merged (tempFileCount/2 merge files)
	//and -1 because the file names start at 0 
	if(tempFileCount%2)
	{
		mergeFiles(string("temp")+to_string(tempFileCount-1)+".db",string("merge")+to_string(tempFileCount/2-1)+".db");
	}

	return true;
}

//reads a subset of database determined by memory capacity into a DoublyLinkedList
//subsequently sorts the list, iterates through it to merge entries with duplicate primary fields separated by delim
//and concatenate secondary fields using conn as the separator
//and exports it into a temp file
//returns the count of temporary files
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

		// cout << lineRead << endl;

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
	return tempFileCount-1;
}

//merges two files
//eliminates duplicate entries by comparing substrings up to the first occurance of delim
//then combines the lines after delim by concatenating with conn
bool mergeFiles(const string &f1, const string &f2, const char delim, const char conn)
{
	static int mergeCount = 0;
	ifstream file1(f1), file2(f2);
	ofstream mergedFile(string("merge") + to_string(mergeCount++) + ".db");
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
			
			line1 += buffer1;
			while(file1.fail() && !file1.eof())
			{
				line1 += buffer1;
				file1.clear();
				file1.getline(buffer1,LINE_BUFFER_SIZE,'\n');
			}
		}

		if(line2 == "")
		{
			file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			
			if(file2.eof())
			{
				break;
			}

			line2 += buffer2;
			while(file2.fail())
			{
				line2 += buffer2;
				file2.clear();
				file2.getline(buffer2,LINE_BUFFER_SIZE,'\n');
			}
		}

		static int delim1, delim2;
		delim1 = line1.find(delim);
		delim2 = line2.find(delim);

		if(line1.substr(0,delim1) == line2.substr(0,delim2))
		{
			line1 += conn + line2.substr(delim2+1);
			line2 = "";
		}
		else if(line1.substr(0,delim1) > line2.substr(0,delim2))
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

	return true;
}

int main(int argc, char const *argv[])
{
	sortDatabase(string("test.db"));
	return 0;
}