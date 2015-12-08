#ifndef POLLINGHANDLER_H
#define POLLINGHANDLER_H

#include "DoublyLinkedList.h"

struct InputPin
{
	uint8_t pin;
	uint32_t time;
};

DoublyLinkedList<InputPin> inputQueue;

void pollForInput()
{
	//poll for which button is pressed and call an interrupt function to handle the button pressed
	static InputPin input;
	if(digitalRead(PREVIOUS_SONG))
	{
		input.pin = PREVIOUS_SONG;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(NEXT_SONG))
	{
		input.pin = NEXT_SONG;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(PREVIOUS_MENU_ITEM))
	{
		input.pin = PREVIOUS_MENU_ITEM;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(NEXT_MENU_ITEM))
	{
		input.pin = NEXT_MENU_ITEM;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(PLAY_SELECT))
	{
		input.pin = PLAY_SELECT;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(PREVIOUS_MENU))
	{
		input.pin = PREVIOUS_MENU;
		input.time = millis();
		inputQueue.add(input);
	}
	if(digitalRead(SWITCH_VIEW_MODE))
	{
		input.pin = SWITCH_VIEW_MODE;
		input.time = millis();
		inputQueue.add(input);
	}
	executeInput();
}

//executes the inputs in the queue
void executeInput()
{
	static InputPin input;
	while(inputQueue.size() > 0)
	{
		input = inputQueue.getHead();
		switch(input.pin)
		{
			case PREVIOUS_SONG:
				Serial.println("PREVIOUS_SONG");
				break;
			case NEXT_SONG:
				Serial.println("NEXT_SONG");
				break;
			case PREVIOUS_MENU_ITEM:
				Serial.println("PREVIOUS_MENU_ITEM");
				break;
			case NEXT_MENU_ITEM:
				Serial.println("NEXT_MENU_ITEM");
				break;
			case PLAY_SELECT:
				Serial.println("PLAY_SELECT");
				break;
			case PREVIOUS_MENU:
				Serial.println("PREVIOUS_MENU");
				break;
			case SWITCH_VIEW_MODE:
				Serial.println("SWITCH_VIEW_MODE");
				break;
			default:
		}
	}
}

#endif