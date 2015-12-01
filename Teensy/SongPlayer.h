#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <string.h>

uint32_t startTime = 0;

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