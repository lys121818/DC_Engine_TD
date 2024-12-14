#pragma once
#include <DC_Event.h>

//Inlcude from STL
#include <string>

class PlaySoundEvent : public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0x77afb639;

public:
	PlaySoundEvent(std::string filePath) : m_fileName(filePath) {}
	~PlaySoundEvent() {}

	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return "PlaySound"; }

	std::string GetFileName() { return m_fileName; }

private:
	std::string m_fileName;

};