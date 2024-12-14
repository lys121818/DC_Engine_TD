#include "DC_Audio.h"
#include "DC_SDLAudio.h"

namespace DC_Engine
{
    std::unique_ptr<DC_Audio> DC_Engine::DC_Audio::Create()
    {
        return std::make_unique<DC_SDLAudio>();
    }
}
