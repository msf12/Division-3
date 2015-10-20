#include <SdFat.h>

#define LFN_LENGTH 250
#define SD_CS_PIN SS

SdFat sd;

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

}

void loop()
{
	SdFile file;
	int count = 0;

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
			Serial.println(c);
			break;
		}
		else
		{
			Serial.println("Invalid input");
		}
	}

	//THIS DOESN'T WORK FOR SOME REASON?????????????

	sd.vwd()->rewind();
	file.rewind();
	for(char i = '0'; c < i && file.openNext(sd.vwd(),O_READ);i++);

	file.printName();
	Serial.println();

}