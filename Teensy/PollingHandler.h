#ifndef POLLINGHANDLER_H
#define POLLINGHANDLER_H

#include "DoublyLinkedList.h"
#include <Bounce2.h>

struct InputPin
{
	uint8_t pin;
	uint32_t time;
};

DoublyLinkedList<InputPin> inputQueue;

Bounce PrevSo_debouncer;
Bounce NextSo_debouncer;
Bounce PrevIt_debouncer;
Bounce NextIt_debouncer;
Bounce PlaySe_debouncer;
Bounce PrevMe_debouncer;
Bounce SwitVi_debouncer;

void pollingHandlerSetup()
{
	PrevSo_debouncer = Bounce();
	NextSo_debouncer = Bounce();
	PrevIt_debouncer = Bounce();
	NextIt_debouncer = Bounce();
	PlaySe_debouncer = Bounce();
	PrevMe_debouncer = Bounce();
	SwitVi_debouncer = Bounce();

	PrevSo_debouncer.attach(PREVIOUS_SONG);
	NextSo_debouncer.attach(NEXT_SONG);
	PrevIt_debouncer.attach(PREVIOUS_MENU_ITEM);
	NextIt_debouncer.attach(NEXT_MENU_ITEM);
	PlaySe_debouncer.attach(PLAY_SELECT);
	PrevMe_debouncer.attach(PREVIOUS_MENU);
	SwitVi_debouncer.attach(SWITCH_VIEW_MODE);

	PrevSo_debouncer.interval(10);
	NextSo_debouncer.interval(10);
	PrevIt_debouncer.interval(10);
	NextIt_debouncer.interval(10);
	PlaySe_debouncer.interval(10);
	PrevMe_debouncer.interval(10);
	SwitVi_debouncer.interval(10);
}

void pollForInput()
{
	static InputPin input;
	PrevSo_debouncer.update();
	NextSo_debouncer.update();
	PrevIt_debouncer.update();
	NextIt_debouncer.update();
	PlaySe_debouncer.update();
	PrevMe_debouncer.update();
	SwitVi_debouncer.update();
	
	if(PrevSo_debouncer.read())
	{
		input.pin = PREVIOUS_SONG;
		input.time = millis();
		inputQueue.add(input);
	}
	if(NextSo_debouncer.read())
	{
		input.pin = NEXT_SONG;
		input.time = millis();
		inputQueue.add(input);
	}
	if(PrevIt_debouncer.read())
	{
		input.pin = PREVIOUS_MENU_ITEM;
		input.time = millis();
		inputQueue.add(input);
	}
	if(NextIt_debouncer.read())
	{
		input.pin = NEXT_MENU_ITEM;
		input.time = millis();
		inputQueue.add(input);
	}
	if(PlaySe_debouncer.read())
	{
		input.pin = PLAY_SELECT;
		input.time = millis();
		inputQueue.add(input);
	}
	if(PrevMe_debouncer.read())
	{
		input.pin = PREVIOUS_MENU;
		input.time = millis();
		inputQueue.add(input);
	}
	if(SwitVi_debouncer.read())
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