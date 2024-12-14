#include "Level.h"

// Include from DontCrashEngine
#include "GameObject.h"
#include "Resource.h"
#include "TileSetComponent.h"
#include "ApplicationLayer.h"

// Include from tinyxml2
#include "tinyxml2.h"

// Include from STL
#include <filesystem>
#include <assert.h>

namespace DC_Engine
{
	std::shared_ptr<GameObject> DC_Engine::Level::LoadTiles(ResourceFile* pResourceFile, std::string filePath)
	{
		auto& log = ApplicationLayer::GetInstance()->GetLog();

		auto pLevel = pResourceFile->LoadResource(filePath);

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.Parse(pLevel->GetData().data(), pLevel->GetData().size());

		if (error != tinyxml2::XML_SUCCESS)
		{
			log.LogError("It was unable to load data from tiles resource file");
			return nullptr;
		}

		// Create map
		tinyxml2::XMLElement* pRoot = doc.FirstChildElement();

		if (strcmp(pRoot->Name(), "map") == 0)
		{
			m_tileWidth = pRoot->UnsignedAttribute("tilewidth");
			m_tileHeight = pRoot->UnsignedAttribute("tileheight");
			for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				if (strcmp(pElement->Name(), "tileset") == 0)
				{
					TileSet tileSet;
					tileSet.Init(pElement, pResourceFile, filePath);
					m_tileDatas.emplace_back(tileSet);
				}
				else if (strcmp(pElement->Name(), "layer") == 0)
				{
					m_layer.Init(pElement);
				}
			}
		}
		else
		{
			log.LogError("It was unable to find 'map' from resource file");
			return nullptr;
		}

		// Create GameObject
		auto pGameObject = std::make_shared<GameObject>(ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetObjectFactory().GetNextObjectId());

		auto& tiles = m_layer.GetTiles();

		auto pTile = std::make_unique<TileSetComponent>(pGameObject.get(), " TileSetComponent");

		pTile->Init((uint32_t)tiles.size());

		size_t tileSetCount = m_tileDatas.size();

		for (size_t tileIndex = 0; tileIndex < tiles.size(); ++tileIndex)
		{
			uint32_t tile = tiles[tileIndex];

			// empty if false
			if (tile > 0)
			{
				bool isTileSetFound = false;
				size_t currentTileSet = 0;

				// Find correct TileSet for the tile index
				while (!isTileSetFound && (currentTileSet < tileSetCount))
				{
					if (tile > m_tileDatas[currentTileSet].GetLastgid())
					{
						tile -= m_tileDatas[currentTileSet].GetLastgid();
						currentTileSet++;
					}
					else
						break;
				}

				// valid index
				assert(currentTileSet < tileSetCount);

				// Set Tile Info
				TileSetComponent::Tile tileInfo;

				// Image source to use
				tileInfo.imageSource = m_tileDatas[currentTileSet].GetImageSource();

				std::replace(tileInfo.imageSource.begin(), tileInfo.imageSource.end(), '\\', '/');

				size_t pos = tileInfo.imageSource.find_last_of("/");

				if (pos != std::string::npos)
				{
					tileInfo.imageSource = tileInfo.imageSource.substr(pos + 1);
				}

				// offset position and size of tile image

				m_tileDatas[currentTileSet].GetSrc(tile, tileInfo.offsetInfo.position.x, tileInfo.offsetInfo.position.y);

				int32_t layerX = tile % m_tileDatas[currentTileSet].GetTileColums() - 1;
				int32_t layerY = tile / m_tileDatas[currentTileSet].GetTileColums();

				tileInfo.offsetInfo.width = m_tileDatas[currentTileSet].GetTileWidth();
				tileInfo.offsetInfo.height = m_tileDatas[currentTileSet].GetTileHeight();
				tileInfo.offsetInfo.position.x = (float)(layerX * m_tileDatas[currentTileSet].GetTileWidth());
				tileInfo.offsetInfo.position.y = (float)(layerY * m_tileDatas[currentTileSet].GetTileWidth());

				size_t tileX = tileIndex % m_layer.GetWidth();
				size_t TileY = tileIndex / m_layer.GetWidth();

				tileInfo.tileRectInfo.width = m_tileWidth;
				tileInfo.tileRectInfo.height = m_tileHeight;
				tileInfo.tileRectInfo.position.x = (float)(tileX * m_tileWidth);
				tileInfo.tileRectInfo.position.y = (float)(TileY * m_tileHeight);

				// Set moveable Tiles
				if ((tile >= 7 && tile <= 9)
					|| tile == 24 || tile == 26
					|| (tile >= 41 && tile <= 43))
				{
					tileInfo.isMoveable = true;
				}

				pTile->AddTiles(tileInfo);
			}
		}

		pGameObject->AddComponent(std::move(pTile));

		return pGameObject;
	}
}
