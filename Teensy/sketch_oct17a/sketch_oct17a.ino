#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>

// sd card chip select pin.
const uint8_t sd_CS_PIN = SS;

unsigned long baud = 19200;
SdFat sd;
SdFile file;
String path;

void setup() {
	path = "/";
	Serial.begin(baud);
	while(!Serial);
	Serial.println("Press the any key");
	while(!Serial.available());

	if (!sd.begin(sd_CS_PIN, SPI_HALF_SPEED)) {
		sd.initErrorHalt();
}

sd.ls();
sd.chdir("Music");


char test[160];
file.getName(test,160);
Serial.print("Test of LFN ")
Serial.println(test); 


char temp[13];
sd.vwd()->getSFN(temp);
path += temp;
path += "/";
Serial.println("Path is " + path);

sd.ls();
sd.chdir("Seth Ect - DiMethylTriptamine");

sd.vwd()->getSFN(temp);
path += temp;
path += "/";
Serial.println("Path is " + path);

while(file.openNext(sd.vwd(),O_READ))
{
	Serial.println();
	char test[160];
	file.getName(test,160);
	Serial.print("Test of LFN ");
	Serial.println(test); 


	file.printFileSize(&Serial);
	Serial.write(' ');
	file.printModifyDateTime(&Serial);
	Serial.write(' ');
	file.printName(&Serial);
	
	if (file.isDir()) {
		// Indicate a directory.
		Serial.write('/');
	}
	Serial.println();
	file.close();
}
Serial.println("Done");
}

void loop() {}
