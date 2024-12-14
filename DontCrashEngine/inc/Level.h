#pragma once

// Include from DontCrashEngine
#include <TileSet.h>
#include <Layer.h>

// Include from STL
#include <memory>
#include <vector>
#include <string>

namespace DC_Engine
{
	class GameObject;
	class ResourceFile;

	class Level
	{
	// Functions
	public:
		std::shared_ptr<GameObject> LoadTiles(ResourceFile* pResourceFile, std::string filePath);

	// Variables
	private:
		std::vector<TileSet> m_tileDatas;
		Layer m_layer;
		uint32_t m_tileWidth;
		uint32_t m_tileHeight;
	};
}

