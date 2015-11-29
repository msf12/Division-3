#include <SdFat.h>
#include <SdFatUtil.h>
#include "DoublyLinkedList.h" //based on code from http://www.codeproject.com/Articles/668818/Implementing-a-Doubly-Linked-List-to-be-used-on-an
#include "FileInfo.h"
#include "SPI.h"
#include "ILI9341_t3.h"

#ifndef LFN_LENGTH
#define LFN_LENGTH 250
#endif
#define SD_CS_PIN SS

//SdFat SD card
SdFat SD;

// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

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
	tft.begin();
	tft.fillScreen(ILI9341_WHITE);
	tft.setTextColor(ILI9341_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);
	
	if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
		tft.println("Error: SD not found");
		SD.initErrorHalt();
	}

	Serial.begin(9600);
	while(!Serial);
	Serial.println("Press the any key");
	tft.println("Press the any key");
	while(!Serial.available());

	SD.chdir("/Music");

	populateFiles();
	// printDir();
}

void loop()
{
}

void clearSerialBuffer()
{
	//clear Serial buffer
	while(Serial.available())
	{
		Serial.read();
	}
}

void populateFiles()
{
	chosenFile = 0;
	if(files.getSize())
		files.clearList();

	SdFile file;
	uint16_t count = 0;
	while(file.openNext(SD.vwd(),O_READ))
	{
		static FileInfo info;
		static char fileName[250];

		file.getName(fileName,LFN_LENGTH);
		info.fileName = new char[strlen(fileName)+1];
		strcpy(info.fileName,fileName);

		if(file.isDir())
		{
			SdFile nestedFile;
			uint32_t numFiles = 0;
			while(nestedFile.openNext(&file,O_READ))
			{
				numFiles++;
				nestedFile.close();
			}
			file.rewind();
			char* files[numFiles];
			for(int i = 0; nestedFile.openNext(&file,O_READ); i++)
			{
				file.getName(fileName,LFN_LENGTH);
				files[i] = new char[strlen(fileName)+1];
				strcpy(files[i],fileName);
			}
		}

		Serial.println(info.fileName);
		Serial.println(count++);

		files.add(info);
		file.close();
	}

	files.sort();
}

void printDir()
{
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(0, 0);
	for(int i=0; i<files.getSize(); i++)
	{
		if(i == chosenFile)
		{
			Serial.print("* ");
			tft.print("* ");
		}
		else
		{
			Serial.print("  ");
			tft.print("  ");
		}
		Serial.println(files.getAt(i)->fileName);
		tft.println(files.getAt(i)->fileName);
	}
	Serial.println();
}

bool changeDir()
{
	SdFile newDir;
	FileInfo file = *files.getAt(chosenFile);
	String temp2 = path + '/' + file.fileName;
	
	newDir.open(SD.vwd(),temp2.c_str(),O_READ);

	if(!newDir.isDir())
		return false;

	// Serial.println(path);
	
	char temp[LFN_LENGTH];
	newDir.getName(temp,LFN_LENGTH);

	if(depth)
		path += "/";
	pathSplits[depth++] = path.length();
	path += temp;

	// Serial.println(path);
	
	SD.chdir(path.c_str());
	return true;
}