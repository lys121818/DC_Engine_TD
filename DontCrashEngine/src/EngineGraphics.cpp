#include "EngineGraphics.h"

#include "DC_SDLRenderer.h"

namespace DC_Engine
{

    std::unique_ptr<EngineGraphics> DC_Engine::EngineGraphics::Create()
    {
        return std::make_unique<DC_SDLRenderer>();
    }

}