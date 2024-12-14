#pragma once
// Include from DontCrashEngine
#include <DC_Event.h>
#include <DC_Rect.h>


class SpawnBulletEvent : public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0x16dce851;

public:
	SpawnBulletEvent(const char* pXMLFile, const DC_Engine::Rect& ownerRect);
	~SpawnBulletEvent() {};

	DC_Engine::EventType GetEventType() const override{ return kEventType; }
	const char* GetName() const override { return "SpawnBullet"; }

	const char* GetXMLFile() const { return m_pXMLFile; }
	DC_Engine::Rect GetOwnerRect() const { return m_rect; }

//Functions
private:
	const char* m_pXMLFile;
	DC_Engine::DC_View::Id m_viewType;
	DC_Engine::Rect m_rect;
};

