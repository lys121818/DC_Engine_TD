#pragma once
#include <string>
#include <unordered_map>
#include <memory>

struct Mix_Chunk;
struct Mix_Channel;

namespace DC_Engine
{
	class Resource;

	class DC_Audio
	{
	public:
		virtual ~DC_Audio() {}

		virtual bool Init() = 0;
		virtual void Destory() = 0;

		virtual bool MusicPlay(std::shared_ptr<Resource>) = 0;
		virtual bool SoundPlay(std::shared_ptr<Resource>) = 0;
		virtual void ControlSoundVolum(const int& amount) = 0;
		virtual void ControlMusicVolum(const int& amount) = 0;
		virtual void ControlMasterVolum(const int& amount) = 0;
		virtual void ToggleMusic() = 0;

		static std::unique_ptr<DC_Audio> Create();

	protected:
		int m_masterVolume = 10;	// Max master volume is 100 and Mix master volume is 0
		int m_musicVolume = 10;
		int m_soundVolume = 10;
	};
}