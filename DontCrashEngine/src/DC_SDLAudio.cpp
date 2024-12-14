#include "DC_SDLAudio.h"
#include "ApplicationLayer.h"
#include "Resource.h"
#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <fstream>
#include <Windows.h>
namespace DC_Engine
{
    DC_SDLAudio::DC_SDLAudio()
        :
        m_initialized(false),
        m_pMusic(nullptr, &Mix_FreeMusic)
    {
    }
    DC_SDLAudio::~DC_SDLAudio()
    {
        Destory();
    }
    
    bool DC_SDLAudio::Init()
    {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO))
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to Initialize SDL_Audio");
            return false;
        }
        m_initialized = true;

        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024))
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to open SDL_mixer");
            return false;
        }

        int flags = MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_OGG | MIX_INIT_WAVPACK;
        int initialized = Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_OGG | MIX_INIT_WAVPACK);
        if (flags != initialized)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to initialize all requested fomats of SDL_mixer");
            return false;
        }

        Mix_VolumeMusic(10);

        return true;
    }

    bool DC_SDLAudio::MusicPlay(std::shared_ptr<Resource> pResource)
    {
        if (!pResource)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Unable to get resource");
            return false;
        }

        // copy data to local resource
        m_musicResource = pResource;

        ApplicationLayer::GetInstance()->GetLog().Log("Muisc Playing: " + m_musicResource->GetName());

        // Halt before play new music
        if (m_isPlaying)
        {
            Mix_HaltMusic();
            m_pMusic.reset();
            m_isPlaying = false;
        }

        // Use SDL_RWops to create music from the Audio data in memory
        SDL_RWops* rw = SDL_RWFromConstMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size() * sizeof(char)));
        if (!rw)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to create RWops for Audio: " + m_musicResource->GetName());
            return false;
        }
        
        m_pMusic = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(Mix_LoadMUS_RW(rw, 0), &Mix_FreeMusic);
        
        //Error Check
        if (m_pMusic.get() == nullptr)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to load music: " + m_musicResource->GetName() + ": " + Mix_GetError());
            m_isPlaying = false;
            m_pMusic.reset();
            return m_isPlaying;
        }
        


        // m_pMusic as struct
        if (Mix_PlayMusic(m_pMusic.get(), -1))
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to play music: " + m_musicResource->GetName() + ": " + Mix_GetError());
            m_isPlaying = false;
            m_pMusic.reset();
            return m_isPlaying;
        }

        // close after the music is playing
        SDL_RWclose(rw);
        m_isPlaying = true;
        return m_isPlaying;
    }

    bool DC_SDLAudio::SoundPlay(std::shared_ptr<Resource> pResource)
    {
        if (!pResource)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Unable to get resource");
            return false;
        }

        ApplicationLayer::GetInstance()->GetLog().Log("Sound Playing: " + pResource->GetName());

        // Use SDL_RWops to create music from the Audio data in memory
        SDL_RWops* rw = SDL_RWFromMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
        if (!rw)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to create RWops for Audio: " + pResource->GetName());
            return false;
        }

        auto soundIter = m_pSounds.find(pResource->GetName());

        // Add sounds to unordered map
        if (soundIter == m_pSounds.end())
        {
            Mix_Chunk* pChunk = Mix_LoadWAV_RW(rw,1);

            //Error Check
            if (pChunk != nullptr)
            {
                soundIter = m_pSounds.emplace(pResource->GetName(), std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(pChunk, &Mix_FreeChunk)).first;
            }
            else
            {
                ApplicationLayer::GetInstance()->GetLog().LogError("Failed to load sound: " + pResource->GetName() + ": " + Mix_GetError());
                return false;
            }
        }

        Mix_PlayChannel(-1, soundIter->second.get(), 0);
        Mix_VolumeChunk(soundIter->second.get(), m_soundVolume);

        return true;
    }
    void DC_SDLAudio::ControlSoundVolum(const int& amount)
    {
        // Set Music Volume
        int volume = m_soundVolume + amount;

        if (volume > MIX_MAX_VOLUME)
            volume = MIX_MAX_VOLUME;
        else if (volume < 0)
            volume = 0;

        m_soundVolume = volume;

        // Apply Master volume to the music volume
        volume = static_cast<int>(m_masterVolume * m_soundVolume / 100.0f);

        for (auto& Iter : m_pSounds)
            Mix_VolumeChunk(Iter.second.get(), volume);
    }
    void DC_SDLAudio::ControlMusicVolum(const int& amount)
    {
        // Set Music Volume
        int volume = m_musicVolume + amount;

        if (volume > MIX_MAX_VOLUME)
            volume = MIX_MAX_VOLUME;
        else if (volume < 0)
            volume = 0;

        m_musicVolume = volume;

        // Apply Master volume to the music volume
        volume = static_cast<int>(m_masterVolume * m_musicVolume / 100.0f);
        Mix_VolumeMusic(volume);
    }

    void DC_SDLAudio::ControlMasterVolum(const int& amount)
    {
        // Setting master volume
        int volume = m_masterVolume + amount;
        if (volume > 100)
            volume = 100;
        else if (volume < 0)
            volume = 0;

        m_masterVolume = volume;

        // Applying the volums to Music and Chunk
        // Music
        volume = static_cast<int>(m_musicVolume * m_masterVolume / 100.0f); // Volume percentage
        Mix_VolumeMusic(volume);

        // Chunk
        volume = static_cast<int>(m_soundVolume * m_masterVolume / 100.0f);
        for (auto& Iter : m_pSounds)
            Mix_VolumeChunk(Iter.second.get(), volume);

    }



    void DC_SDLAudio::ToggleMusic()
    {
        if (m_isPlaying)
        {
            Mix_PauseMusic();
            m_isPlaying = false;
        }
        else
        {
            Mix_ResumeMusic();
            m_isPlaying = true;
        }
    }
    void DC_SDLAudio::Destory()
    {
        if (m_isPlaying)
        {
            Mix_HaltMusic();
            m_isPlaying = false;
        }

        m_pMusic.reset();
        m_pSounds.clear();

        if (m_initialized)
        {
            Mix_CloseAudio();
            Mix_Quit();
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }
    }
}