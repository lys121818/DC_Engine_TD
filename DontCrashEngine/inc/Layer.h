#pragma once

// Include from tinyxml2
#include <tinyxml2.h>

// Include from STL
#include <vector>

namespace DC_Engine
{
	class Layer
	{
	// Functions
	public:
		Layer();

		bool Init(tinyxml2::XMLElement* pElement);

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

		auto& GetTiles() const { return m_tiles; }

	// Variables
	private:
		std::vector<uint32_t> m_tiles;
		uint32_t m_width;
		uint32_t m_height;
	};
}
