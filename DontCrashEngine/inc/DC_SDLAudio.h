#pragma once
#include "DC_Audio.h"

#include <memory>
#include <string>
#include <unordered_map>

struct _Mix_Music;

struct Mix_Chunk;
struct Mix_Channel;

struct SDL_RWops;

namespace DC_Engine
{

	class DC_SDLAudio : public DC_Audio
	{
	public:
		DC_SDLAudio() ;
		~DC_SDLAudio();

		bool Init() override;
		bool MusicPlay(std::shared_ptr<Resource> pResource) override;
		bool SoundPlay(std::shared_ptr<Resource> pResource) override;

		// #DG: nitpick, but this should be "Volume"
		void ControlSoundVolum(const int& amount) override;
		void ControlMusicVolum(const int& amount) override;
		void ControlMasterVolum(const int& amount) override;

		void ToggleMusic() override;

		void Destory() override;

	private:
		bool m_initialized = false;

		std::shared_ptr<Resource> m_musicResource;

		// Have m_pMusic as struct
		std::unique_ptr<_Mix_Music, void(*)(_Mix_Music*)> m_pMusic;

		std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)>> m_pSounds;


		bool m_isPlaying = false;	// toggle for the music
	};
}

