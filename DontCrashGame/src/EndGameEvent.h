#pragma once
#include <DC_Event.h>
class EndGameEvent : public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0x7d2915d;

public:
	EndGameEvent() {};
	~EndGameEvent() {};

	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return "EndGame"; }

	//Functions
private:
};