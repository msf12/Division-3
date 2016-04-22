#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <Audio.h>
#include <SerialFlash.h>
#include <Wire.h>
extern SdFat SD;

#include <play_sd_flac.h>
#include <play_sd_mp3.h>
#include <play_sd_aac.h>

//Audio stream and connection definitions
//https://github.com/PaulStoffregen/Audio
//https://github.com/FrankBoesing/Arduino-Teensy-Codec-lib/
AudioPlaySdFlac	playFlac;
AudioPlaySdMp3	playMp3;
AudioPlaySdAac	playAac;
AudioOutputI2S  i2s;
AudioConnection patchCord1(playFlac, 0, i2s, 0);
AudioConnection patchCord2(playFlac, 1, i2s, 1);
AudioConnection patchCord3(playMp3, 0, i2s, 0);
AudioConnection patchCord4(playMp3, 1, i2s, 1);
AudioConnection patchCord5(playAac, 0, i2s, 0);
AudioConnection patchCord6(playAac, 1, i2s, 1);
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
		return true;
	}

	bool isPlaying ()
	{
		return true;
	}

	bool pausePlayback ()
	{
		return true;
	}

	bool resumePlayback ()
	{
		return true;
	}

	bool updateVolume (int newVolume)
	{
		return sgtl5000.volume(newVolume);
	}

	bool setup()
	{
		AudioMemory(6);
		sgtl5000.enable();
		sgtl5000.volume(0.2);
		SPI.setMOSI(7);
		SPI.setSCK(14);
		return true;
	}
	
};

#endif // AUDIOHANDLER_H
