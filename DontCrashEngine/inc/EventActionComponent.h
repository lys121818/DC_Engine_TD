#pragma once
#include <DC_ObjectComponent.h>
class EventActionComponent : public DC_Engine::DC_ObjectComponent
{
public:
	EventActionComponent(DC_Engine::GameObject* pOwner, const std::string& name);
	~EventActionComponent() {}

	bool Init(tinyxml2::XMLElement* pData) override;

	uint32_t GetEventId()const { return m_eventId; }
private:
	uint32_t m_eventId;
};

