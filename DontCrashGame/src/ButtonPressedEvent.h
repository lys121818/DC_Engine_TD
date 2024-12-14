#pragma once
#include <DC_Event.h>
class ButtonPressedEvent :public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0xca7390fd;
	using EventId = uint32_t;
public:
	ButtonPressedEvent(EventId id) :m_id(id) {}
	~ButtonPressedEvent() {}

	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return "ButtonPressed"; }

	EventId GetEventIdToProcess() { return m_id; }
private:
	EventId m_id;
};