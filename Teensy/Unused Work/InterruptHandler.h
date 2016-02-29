#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

void interruptHandlerSetup()
{
	//User Input interrupts
	attachInterrupt(PREVIOUS_SONG, previousSongInterrupt, RISING);
	attachInterrupt(NEXT_SONG, nextSongInterrupt, RISING);
	// attachInterrupt(PREVIOUS_SONG, backwardsTrackingInterrupt, LOW);
	// attachInterrupt(NEXT_SONG, forwardsTrackingInterrupt, LOW);
	attachInterrupt(PREVIOUS_MENU_ITEM, previousItemInterrupt, FALLING);
	attachInterrupt(NEXT_MENU_ITEM, nextItemInterrupt, FALLING);
	attachInterrupt(PLAY_SELECT, playSelectInterrupt, FALLING);
	attachInterrupt(PREVIOUS_MENU, previousMenuInterrupt, FALLING);
	// attachInterrupt(SWITCH_VIEW_MODE, switchViewInterrupt, FALLING);
}

#endif