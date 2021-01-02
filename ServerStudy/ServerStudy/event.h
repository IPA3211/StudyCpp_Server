#pragma once

#include <windows.h>

semaphore
class Event
{
public:
	HANDLE pEvent;
	Event();
	~Event();
	void Wait();
	void cvEvent();
	void cvEvent(int& i);

private:

};

Event::Event()
{
	pEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

Event::~Event()
{
}

void Event::Wait() {
	WaitForSingleObject(pEvent, INFINITE);
}

void Event::cvEvent() {
	SetEvent(pEvent);
	setEv
}