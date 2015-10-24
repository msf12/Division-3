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
	uint16_t index;
	//pointer to an array to be created at runtime
	char* longFileName;
};

//pointer to array of files in current directory to be built at runtime
FileInfo* files;

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
}



void loop()
{
	
}