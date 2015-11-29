#include <SdFat.h>

#define LFN_LENGTH 250

// sd card chip select pin.
const uint8_t SD_CS_PIN = SS;

unsigned long baud = 19200;
SdFat sd;
String path;

void lsDir(bool root = false)
{
	SdFile file;
	char lfn[160]; //char[] to hold the long file names
	//store the long file name of the working directory in lfn
	sd.vwd()->getName(lfn,LFN_LENGTH);

	//store length of the path to remove added parts later
	int pathLength = path.length();
	
	// Serial.print("Current vwd is ");
	// sd.vwd()->printName(&Serial);
	Serial.print("\nCurrent path is ");

	if(!root) //root path + lfn would be "//"
	{
		Serial.println(path + lfn + "/");
		// Serial.println("DEBUG: NOT ROOT DIRECTORY");
		path += lfn;
		path += "/";
	}
	else
	{
		Serial.println(path.c_str());
	}

	//store the current working directory using absolute path
	SdFile currentDir(path.c_str(),O_READ);

	// Serial.print("currentDir: ");
	// size_t ptr = (size_t)(&currentDir);
	// Serial.println(ptr);
	// currentDir.printName(&Serial);
	// Serial.println("<-");

	while(file.openNext(&currentDir,O_READ)) //loop through the contents of currentDir
	{
		file.printName(&Serial); //print the file names

		if (file.isDir()) {
			// Indicate a directory.
			Serial.write('/');

			//recursively ls the directory entries
			file.getName(lfn,LFN_LENGTH);
			Serial.print("\nDirectory found: entering ");
			Serial.println(lfn);
			sd.chdir(lfn);
			lsDir();
			sd.chdir(path.c_str());
		}
		Serial.println();
		file.close();

		// //Print the address of the currentDir object
		// //THIS VALUE IS CHANGING SOMEWHERE SOMEHOW
		// Serial.print("currentDir: ");
		// size_t ptr = (size_t)(&currentDir);
		// Serial.println(ptr);
		// currentDir.printName(&Serial);
		// Serial.println("<-");

	}

	Serial.println("No files left in directory");

	//restore the path to the parent directory
	path.remove(pathLength);
}

void setup() {

	path = "/";
	SdFile file;
	Serial.begin(baud);
	while(!Serial);
	Serial.println("Press the any key");
	while(!Serial.available());

	if (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
		sd.initErrorHalt();
	}

	sd.chdir(path.c_str());
	lsDir(true);
	// sd.ls();
	// sd.chdir("Music");

	// char temp[160];
	// sd.vwd()->getName(temp,160);
	// path += temp;
	// path += "/";
	// Serial.println("Path is " + path);

	// sd.ls();
	// sd.chdir("Seth Ect - DiMethylTriptamine");

	// sd.vwd()->getName(temp,160);
	// path += temp;
	// path += "/";
	// Serial.println("Path is " + path);

	// while(file.openNext(sd.vwd(),O_READ))
	// {

	// 	file.printFileSize(&Serial);
	// 	Serial.write(' ');
	// 	file.printModifyDateTime(&Serial);
	// 	Serial.write(' ');
	// 	file.printName(&Serial);
		
	// 	if (file.isDir()) {
	// 		// Indicate a directory.
	// 		Serial.write('/');
	// 	}
	// 	Serial.println();
	// 	file.close();
	// }
	Serial.println("Done");
}

void loop() {}
