#include <SdFat.h>

#define LFN_LENGTH 250
#define SD_CS_PIN SS

SdFat sd;
String path;
int pathSplits[2];
uint8_t depth;

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

//returns number of files in the directory
int lsDir()
{
	SdFile file;
	int count = 0;
	//ensure working directory is rewound
	sd.vwd()->rewind();

	while(file.openNext(sd.vwd(),O_READ))
	{
		Serial.print(count);
		Serial.print(" ");
		file.printName(&Serial);

		if(file.isDir())
		{
		    Serial.print("/");
		}

		Serial.println();
		count++;
		file.close();
	}

	if(depth)
	{
		Serial.print(count);
		Serial.println(" Return to previous directory");
	}
	
	return count;
}

void loop()
{
	int count = lsDir();

	Serial.println("Pick a file");

	int input;
	bool ret = false;

	while(1)
	{
		//clear Serial buffer
		while(Serial.available())
		{
			Serial.read();
		}

		while(!Serial.available());

		input = Serial.parseInt();
		Serial.println(input);

		if(input >= 0 && input < count)
		{
			Serial.print("You chose file number ");
			Serial.print(input);
			Serial.print(": ");
			break;
		}
		else if(input == count) //if return to previous directory was chosen
		{
			Serial.println("Returning to previous directory");
			ret = true;
			break;
		}
		else
		{
			Serial.println("Invalid input");
		}
	}

	if(!ret) //user chose a file in the directory
	{
		sd.vwd()->rewind();
		SdFile file;
		file.openNext(sd.vwd(),O_READ);

		//i starts at '/' so if input is 0 it will open the first file
		for(int i = 0; i < input; i++)
		{
			//file closes so that openNext will open the next file
			file.close();
			//openNext happens second so when the selected file is reached the loop ends
			//with the file open
			file.openNext(sd.vwd(),O_READ);
		}

		file.printName(&Serial);
		Serial.println();
		if(file.isDir()) //get name of directory, add it to the path, adjust the depth, and keep track of the new split in the path
		{
			char temp[LFN_LENGTH];
			file.getName(temp,LFN_LENGTH);
			path += "/";
			pathSplits[depth++] = path.length();
			path += temp;
			sd.chdir(path.c_str());
		}
		else
		{
			int data;
			while((data = file.read()) >= 0) //SdFat ReadWriteSdFat example
				Serial.print((char)data);
			Serial.println();
		}
		file.close();
	}
	else //adjust the path and depth and change directories
	{
		path.remove(pathSplits[--depth]);
		sd.chdir(path.c_str());
	}
}