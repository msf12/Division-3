#include <SdFat.h>

#define LFN_LENGTH 250
#define SD_CS_PIN SS

SdFat sd;
String path;

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
	SdFile file;
	int count = 0;

	//ensure working directory is rewound every loop
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
	Serial.println("Pick a file");

	char c;

	while(1)
	{
		//clear Serial buffer
		while(Serial.available())
		{
			Serial.read();
		}

		while(!Serial.available());

		c = Serial.read();

		// Serial.println(c);
		// Serial.println(char('0'+count));

		if(c >= '0' && c < (char)('0'+count))
		{
			Serial.print("You chose file number ");
			Serial.print(c);
			Serial.print(": ");
			break;
		}
		else
		{
			Serial.println("Invalid input");
		}
	}

	sd.vwd()->rewind();

	//i starts at '/' so if c is '0' it will open the first file
	for(char i = '/'; i < c;i++)
	{
		//file closes so that openNext will open the next file
		file.close();
		//openNext happens second so when the selected file is reached the loop ends
		//with the file open
		file.openNext(sd.vwd(),O_READ);
	}

	file.printName(&Serial);
	bool isDir = file.isDir();
	if(isDir)
	{
	    Serial.print("/");
		Serial.println("\nThis file is a directory. Would you like to open the directory?[Y/n]");
	}
	else
		Serial.println("\nWould you like to print the contents of this file?[Y/n]");

	//clear Serial buffer
	while(Serial.available())
	{
		Serial.read();
	}

	while(!Serial.available());

	c = Serial.read();
	if(c == 'y' || c == 'Y')
	{
		if(isDir)
		{
			char temp[LFN_LENGTH];
			file.getName(temp,LFN_LENGTH);
			path += "/";
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
	}
	file.close();
}