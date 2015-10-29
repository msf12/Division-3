#include <SdFat.h>
#include "DoublyLinkedList.h" //based on code from http://www.codeproject.com/Articles/668818/Implementing-a-Doubly-Linked-List-to-be-used-on-an
#include "FileInfo.h"

#ifndef LFN_LENGTH
#define LFN_LENGTH 250
#endif
#define SD_CS_PIN SS

//SdFat SD card
SdFat sd;

//String to hold the current path
String path;

//because the filesystem is of a predetermined organizational structure, arrays
//involving depth have a predetermined number elements

//locations in the string of the '/' characters that split directory names
uint16_t pathSplits[2];

//location in files of the currently selected file
uint16_t chosenFile; //<---------------- figure out way to store positions in each directory

//how deep in the filesystem the user is (used to access the appropriate path split)
uint8_t depth;

//DoublyLinkedList of FileInfo structs to store the files in the current directory
//such that they can be displayed in alphabetical order and opened by index
DoublyLinkedList<FileInfo> files;

//clears the Serial buffer to prepare for new input
void clearSerialBuffer();

//prints the current directory as stored alphabetically in files
//the currently selected file is marked in the format "* FileName"
//prints to the Serial buffer for the moment
void printDir();

//changes to the currently selected subdirectory
//returns false if file is not a directory
bool changeDir();

//erases (if not empty) and populates the files list with the files from the new directory
void populateFiles();

void setup()
{   
	if (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
		sd.initErrorHalt();
	}
	sd.chdir();

	Serial.begin(9600);
	while(!Serial);
	Serial.println("Press the any key");
	while(!Serial.available());
	path = "/";

	populateFiles();
}

void loop()
{
	printDir();
	clearSerialBuffer();

	Serial.println("M - Up a menu\nU - Up\nD - Down\nP - Play song/select directory");

	while(!Serial.available());
	char input = tolower(Serial.read());

	switch (input)
	{
		case 'm':
			if(depth>-1)
			{
				//remove the current directory from the path
				path.remove(pathSplits[--depth]);
				//change to the directory above the current directory using the new path
				sd.chdir(path.c_str());
				populateFiles();
			}
			break;
		case 'u':
			if(chosenFile > 0)
				chosenFile--;
			break;
		case 'd':
			if(chosenFile < files.getSize()-1)
				chosenFile++;
			break;
		case 'p':
			if(changeDir())
			{
				populateFiles();
			}
			else
			{
				//play song if not a directory
			}
			break;
		default:
			Serial.println("Invalid input");
	}
}

void clearSerialBuffer()
{
	//clear Serial buffer
	while(Serial.available())
	{
		Serial.read();
	}
}

void printDir()
{
	for(int i=0; i<files.getSize(); i++)
	{
		if(i == chosenFile)
		{
			Serial.print("* ");
		}
		else
		{
			Serial.print("  ");
		}
		Serial.println(files.getAt(i)->fileName);
	}
	Serial.println();
}

bool changeDir()
{
	SdFile newDir;
	FileInfo file = *files.getAt(chosenFile);
	
	newDir.open(sd.vwd(),file.index,O_READ);

	if(!newDir.isDir())
		return false;

	Serial.println(path);
	char temp[LFN_LENGTH];
	newDir.getName(temp,LFN_LENGTH);

	if(depth)
		path += "/";
	pathSplits[depth++] = path.length();
	path += temp;
	Serial.println(path);
	sd.chdir(path.c_str());
	return true;
}

void populateFiles()
{
	chosenFile = 0;
	if(files.getSize())
		files.clearList();

	SdFile file;
	while(file.openNext(sd.vwd(),O_READ))
	{
		FileInfo info;

		file.getName(info.fileName,LFN_LENGTH);
		info.index = sd.vwd()->curPosition()/32-1; //http://forum.arduino.cc/index.php?topic=154033.0

		// Serial.print(info.fileName);
		// Serial.print(" - ");
		// Serial.println(info.index);
		
		files.add(info);
		file.close();
	}

	files.sort();
}