#pragma once

// Include from DontCrashEngine
#include <DC_Event.h>
#include <string>

// Forward declare
namespace DC_Engine
{
	class DC_View;
}
class CreateGameObjectEvent : public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0x5c8e7649;
	static constexpr const char* s_kTypeName = "CreatObject";
// Functions
public:
	CreateGameObjectEvent(const char* pXMLFile, DC_Engine::DC_View::Id viewType);
	~CreateGameObjectEvent() {};
	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return s_kTypeName; }
	DC_Engine::DC_View::Id GetViewType() const { return m_viewType; }

	const char* GetXMLFile() const { return m_pXMLFile; }

// Variables
private:
	const char* m_pXMLFile;
	DC_Engine::DC_View::Id m_viewType;
};

