#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include "Settings.h"
#include <Audio.h>
#include <SerialFlash.h>
#include <Wire.h>
extern SdFat SD;

#include <play_sd_flac.h>
#include <play_sd_mp3.h>
#include <play_sd_aac.h>

AudioPlaySdFlac	playFlac;
AudioPlaySdMp3	playMp3;
AudioPlaySdAac	playAac;
AudioOutputI2S i2s;
//AudioConnection stuff???

AudioControlSGTL5000	sgtl5000;

uint32_t startTime = 0;

void audioSetup()
{
	AudioMemory(6);
	sgtl5000.enable();
	sgtl5000.volume(0.2);
	SPI.setMOSI(7);
	SPI.setSCK(14);
}

void playNewSong(String s)
{
	startTime = millis();
	/* play the song */
}

uint32_t updateTimePlaying()
{
	return millis()-startTime;
}

#endif