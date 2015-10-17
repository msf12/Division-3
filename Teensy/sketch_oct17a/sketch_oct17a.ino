#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>

// SD card chip select pin.
const uint8_t SD_CS_PIN = SS;

unsigned long baud = 19200;
SdFat SD;
SdFile file;
String path;

void printDirContents(SdFile &file)
{
//  file.chdir
}

void setup() {
  Serial.begin(baud);
  while(!Serial);
  Serial.println("Press the any key");
  while(!Serial.available());

  if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    SD.initErrorHalt();
  }
  
  Serial.println("Path is " + path);
  char temp[13];
  SD.vwd()->getSFN(temp); //get SFN and store it in char[]
  path += temp;
  Serial.println("Path is " + path);
  
  SD.ls();
  SD.chdir("Music");
  
  SD.vwd()->getSFN(temp);
  path += temp;
  path += "/";
  Serial.println("Path is " + path);

  SD.ls();
  SD.chdir("Seth Ect - DiMethylTriptamine");
  
  SD.vwd()->getSFN(temp);
  path += temp;
  path += "/";
  Serial.println("Path is " + path);
  
  while(file.openNext(SD.vwd(),O_READ))
  {
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
