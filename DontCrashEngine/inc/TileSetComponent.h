#pragma once
#include "DC_ObjectComponent.h"
#include "ImageRenderComponent.h"

namespace DC_Engine
{
	class EngineTexture;
	class TileSetComponent : public DC_ObjectComponent
	{
	public:
		struct Tile
		{
			std::string imageSource = "";
			Rect offsetInfo; // offset information of the tile in image source
			Rect tileRectInfo; // rect information of the tile in game
			bool isMoveable = false;
		};

	// Functions
	public:
		TileSetComponent(GameObject* pOwner, std::string name);
		virtual ~TileSetComponent() {}

		bool Init(tinyxml2::XMLElement*) override { return true; }
		bool Init(uint32_t numTiles);
		bool PostInit() override;
		void Render(EngineGraphics* pRenderer) override;

		void AddTiles(const Tile& tile) { m_tiles.emplace_back(tile); }

		bool IsMoveable(size_t index) { return m_tiles[index].isMoveable; }


	// Variables
	private:
		std::vector<Tile> m_tiles;
	};
}

