
#include<SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
SdFat sd;
const uint8_t SD_CS_PIN = SS;
//------------------------------------------------------------------------------
// does not implement directory byte 12 for lowercase bits with 8.3 names.
// bit 0X10 means lowercase extension and bit 0X08 lowercase basename
//
void listLfn(SdBaseFile* dirFile) {
  uint8_t offset[] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
  char name[13];
  char lfn[131];
  bool haveLong = false;
  dir_t dir;
  uint8_t i;
  uint8_t lfnIn = 130;
  uint8_t ndir;
  uint8_t sum;
  uint8_t test;

  dirFile->rewind();
  while (dirFile->read(&dir, 32) == 32) {
    if (DIR_IS_LONG_NAME(&dir)) {
      if (!haveLong) {
        if ((dir.name[0] & 0XE0) != 0X40) continue;
        ndir = dir.name[0] & 0X1F;
        test = dir.creationTimeTenths;
        haveLong = true;
        lfnIn = 130;
        lfn[lfnIn] = 0;
      } else if (dir.name[0] != --ndir || test != dir.creationTimeTenths) {
        haveLong = false;
        continue;
      }
      char *p = (char*)&dir;
      if (lfnIn > 0) {
        lfnIn -= 13;
        for (i = 0; i < 13; i++) {
          lfn[lfnIn + i] = p[offset[i]];
        }
      }
    } else if (DIR_IS_FILE_OR_SUBDIR(&dir) 
      && dir.name[0] != DIR_NAME_DELETED 
      && dir.name[0] != DIR_NAME_FREE) {
      if (haveLong) {
        for (sum = i = 0; i < 11; i++) {
           sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + dir.name[i];
        }
        if (sum != test || ndir != 1) haveLong = false;
      }
      SdFile::dirName(dir, name);
      Serial.print(name);
      if (haveLong) {
        Serial.print("  ");
        Serial.print(lfn + lfnIn);
      } else if (dir.reservedNT) {
        Serial.print(" 8.3 lowercase bits: ");
        Serial.print(dir.reservedNT, HEX);
      }
      Serial.println();
      haveLong = false;
    } else {
      if (dir.name[0] == DIR_NAME_FREE) return;
      haveLong = false;
    }
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  if (!sd.begin(SD_CS_PIN)) sd.initErrorHalt();
  // list files in root directory (volume working directory)
  listLfn(sd.vwd());
  Serial.println("Done");
}
void loop() {}
