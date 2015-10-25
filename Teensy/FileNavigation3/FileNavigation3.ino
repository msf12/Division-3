#include <SdFat.h>
#include "DoublyLinkedList.h" //based on code from http://www.codeproject.com/Articles/668818/Implementing-a-Doubly-Linked-List-to-be-used-on-an

#define LFN_LENGTH 250
#define SD_CS_PIN SS

SdFat sd;
String path;
int pathSplits[2];
uint8_t depth;

//used to store information about files when they are first listed
//http://forum.arduino.cc/index.php?topic=173562.0
struct FileInfo {
	uint32_t index;
	//pointer to an array to be created at runtime
	char fileName[LFN_LENGTH];
};

bool operator==(FileInfo &left, FileInfo &right)
{
	for (uint i = 0; i < sizeof(left.fileName)/sizeof(*(left.fileName)); ++i)
	{
		if(left.fileName[i] != right.fileName[i])
			return false;
	}
	return true;
}

bool operator!=(FileInfo &left, FileInfo &right)
{
	return !(left==right);
}

bool operator<=(FileInfo &left, FileInfo &right)
{
	for (uint i = 0; i < sizeof(left.fileName)/sizeof(*(left.fileName)); ++i)
	{
		if(left.fileName[i] > right.fileName[i])
			return false;
	}
	return true;
}

bool operator>=(FileInfo &left, FileInfo &right)
{
	for (uint i = 0; i < sizeof(left.fileName)/sizeof(*(left.fileName)); ++i)
	{
		if(left.fileName[i] < right.fileName[i])
			return false;
	}
	return true;
}

bool operator<(FileInfo &left, FileInfo &right)
{
	return !(left>=right);
}

bool operator>(FileInfo &left, FileInfo &right)
{
	return !(left<=right);
}

// template <typename FileInfo>;
DoublyLinkedList<FileInfo> files;

void setup()
{
	SdFile file;
	if (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
		sd.initErrorHalt();
	}
	sd.chdir();

	Serial.begin(9600);
	while(!Serial);
	Serial.println("Press the any key");
	while(!Serial.available());
	path = "/";

	while(file.openNext(sd.vwd(),O_READ))
	{
		FileInfo info;

		file.getName(info.fileName,LFN_LENGTH);
		info.index = sd.vwd()->curPosition();

		Serial.print(info.fileName);
		Serial.print(" - ");
		Serial.println(info.index);
		
		files.add(info);
		file.close();
	}

	Serial.println();

	for(int i=0; i<files.getSize(); i++)
	{
		Serial.println(files.getAt(i)->fileName);
	}

	Serial.println();

	files.sort();

	for(int i=0; i<files.getSize(); i++)
	{
		Serial.println(files.getAt(i)->fileName);
	}
}

void loop()
{
	
}