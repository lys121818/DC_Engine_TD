#pragma once
// Include from DontCrashEngine
#include <DC_ObjectComponent.h>

// Include from STL
#include <unordered_map>
#include <memory>

// Forward declare
namespace DC_Engine
{
	class DC_Audio;
	class Resource;
}

class SoundComponent : public DC_Engine::DC_ObjectComponent
{

public:
	SoundComponent(DC_Engine::GameObject* pOwner, const std::string& name);
	~SoundComponent();

	bool Init(tinyxml2::XMLElement* pData) override;

	void PlaySound(std::shared_ptr<DC_Engine::Resource> pResource);

private:
	std::unordered_map<std::string, std::string> m_soundSource;
	DC_Engine::DC_Audio& m_pAudio;
};

