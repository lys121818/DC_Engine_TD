#pragma once
#include <DC_Event.h>

//Inlcude from STL
#include <string>

class PlayMusicEvent : public DC_Engine::Event
{
public:
	static const DC_Engine::EventType kEventType = 0x2af98568;

public:
	PlayMusicEvent(std::string filePath) : m_fileName(filePath) {}
	~PlayMusicEvent() {}

	DC_Engine::EventType GetEventType() const override { return kEventType; }
	const char* GetName() const override { return "PlayMusic"; }
	
	std::string GetFileName() { return m_fileName; }
private:
	std::string m_fileName;
};