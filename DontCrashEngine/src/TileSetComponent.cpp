#include "TileSetComponent.h"

// Include from DontCrashEngine
#include "ApplicationLayer.h"

namespace DC_Engine
{
    TileSetComponent::TileSetComponent(GameObject* pOwner, std::string name)
        :DC_ObjectComponent(pOwner, name)
    {
        
    }
    bool TileSetComponent::Init(uint32_t numTiles)
    {
        m_tiles.reserve(numTiles);
        return true;
    }
    bool TileSetComponent::PostInit()
    {
        return false;
    }
    void TileSetComponent::Render(EngineGraphics* pRenderer)
    {
        for (auto& tile : m_tiles)
        {
            // if tile exist
            if (tile.imageSource != "")
            {
                pRenderer->StartDraw(tile.imageSource, tile.offsetInfo, tile.tileRectInfo, 0);
            }
        }
    }
}
