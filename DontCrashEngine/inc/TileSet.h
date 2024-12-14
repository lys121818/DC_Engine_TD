#pragma once
// Include from DontCrashEngine

// Include from tinyxml2
#include <tinyxml2.h>

// Include from STL
#include <string>
#include <memory>

namespace DC_Engine
{
	class ResourceFile;
	class DC_SDLTexture2D;

	class TileSet
	{
	// Functions
	public:
		TileSet();

		bool Init(tinyxml2::XMLElement* pElement, ResourceFile* pResourceFile, std::string mapFilePath);

		void GetSrc(uint32_t tileIndex, float& srcX, float& srcY);
		auto GetImageSource() const { return m_imageSource; }
		uint32_t GetTileWidth() const { return m_tileWidth; }
		uint32_t GetTileHeight() const { return m_tileHeight; }
		uint32_t GetLastgid() const { return m_lastgid; }
		uint32_t GetTileColums() const { return m_tileColums; }

	private:
		void NormalizeSourcePath();
	// Variables
	private:
		std::string m_imageSource;
		uint32_t m_lastgid;
		uint32_t m_tileWidth;
		uint32_t m_tileHeight;
		uint32_t m_tileColums;
	};
}

