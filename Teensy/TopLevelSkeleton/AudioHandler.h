#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

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

class AudioHandler
{
public:

	AudioHandler ()
	{}

	virtual ~AudioHandler ()
	{}

	bool playSong (String songPath)
	{
	}

	bool isPlaying ()
	{
	}

	bool pausePlayback ()
	{
	}

	bool resumePlayback ()
	{
	}

	bool updateVolume (int newVolume)
	{
	}

	bool setup()
	{
		AudioMemory(6);
		sgtl5000.enable();
		sgtl5000.volume(0.2);
		SPI.setMOSI(7);
		SPI.setSCK(14);
	}
	
};

#endif // AUDIOHANDLER_H
