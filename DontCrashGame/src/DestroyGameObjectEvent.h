#pragma once
#include <DC_Event.h>

// Forward declare
namespace DC_Engine
{
	class DC_View;
	class GameObject;
}

class DestroyGameObjectEvent : public DC_Engine::Event
{
public:
	using Id = uint32_t;
	static const DC_Engine::EventType kEventType = 0xd77a8813;

public:
	DestroyGameObjectEvent(Id id);
	~DestroyGameObjectEvent() {}

	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return "DestroyActor"; }

	Id GetId() const { return m_id; }

private:
	Id m_id;
};

