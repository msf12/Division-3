#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#define LINE_BUFFER_SIZE 100

using std::string; using std::to_string; using std::cout; using std::endl; using std::ifstream; using std::ofstream; using std::fstream;

bool initializeDatabase(string database, const char delim='\t', const char conn=',')
{
	ifstream db(database);
	std::remove("table0.db");
	std::remove("table1.db");

	//create artist-album-albumid albumid-song-songid songid-path
	fstream artalb("table0.db",fstream::in | fstream::out | fstream::trunc);
	fstream albsong("table1.db",fstream::in | fstream::out | fstream::trunc);
	// ofstream songpath("table2.db");

	for(char buffer[LINE_BUFFER_SIZE]; db.getline(buffer,LINE_BUFFER_SIZE,'\n');)
	{
		static string line = "", tempLine = "";
		static string splitLine[3];

		line += buffer;
		if(db.fail())
		{
			db.clear();
		}
		else
		{
			static bool entryFound;
			static int songCount = 0;
			static int ind1, ind2, tempAlbId = 0;
			
			ind1 = line.find(delim);
			ind2 = line.rfind(delim);
			
			splitLine[0] = line.substr(0,ind1);
			splitLine[1] = line.substr(ind1+1,ind2);
			splitLine[2] = line.substr(ind2+1);

			// artalb.seekg(0);

			// entryFound = false;
			// while(artalb.getline(buffer,LINE_BUFFER_SIZE,'\n'))
			// {
			// 	tempLine += buffer;
			// 	if(artalb.fail())
			// 	{
			// 		artalb.clear();
			// 	}
			// 	else
			// 	{
			// 		if(splitLine[0] == tempLine.substr(0,tempLine.find('\t')) &&
			// 			splitLine[1] == tempLine.substr(tempLine.find('\t')+1,tempLine.rfind('\t')))
			// 		{
			// 			entryFound = true;
			// 			tempAlbId = std::stoi(tempLine.substr(tempLine.rfind('\t')+1));
			// 			break;
			// 		}
			// 	}
			// }

			// if(!entryFound)
			// {
				static int albumCount = 0;
			// 	tempAlbId = albumCount;
			// 	artalb.seekg(0,artalb.end);
				artalb << splitLine[0] << '\t' << splitLine[1] << '\t' << albumCount++ << '\n';
			// 	// cout << splitLine[0] << '\t' << splitLine[1] << '\t' << to_string(albumCount++) << endl;
			// }

			// albsong.seekg(0,albsong.end);
			// albsong << tempAlbId << '\t' << splitLine[2] << '\t' << songCount++;
			albsong << splitLine[1] << '\t' << splitLine[2] << '\t' << songCount++ << '\n';

			line = "";
		}
	}	
}

int main(int argc, char const *argv[])
{
	initializeDatabase("test.db");
	return 0;
}