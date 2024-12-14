#include "TileSet.h"

// Include from DontCrashEngine
#include "DC_SDLTexture2D.h"
#include "ApplicationLayer.h"
#include "Resource.h"
#include "ResourceFile.h"

// Include from STL
#include <filesystem>
#include <algorithm>

namespace DC_Engine
{
	TileSet::TileSet()
		:
		m_tileWidth(0),
		m_tileHeight(0),
		m_tileColums(0),
		m_lastgid(0),
		m_imageSource()
	{
	}
	bool TileSet::Init(tinyxml2::XMLElement* pElement, ResourceFile* pResourceFile, std::string mapFilePath)
	{
		const char* source = pElement->Attribute("source");
		auto& log = ApplicationLayer::GetInstance()->GetLog();

		std::replace(mapFilePath.begin(), mapFilePath.end(), '\\', '/');

		size_t pos = mapFilePath.find_last_of("/");

		if (pos != std::string::npos)
		{
			mapFilePath = mapFilePath.substr(0, pos + 1);
		}
		std::string filepath = mapFilePath + source;

		auto pTileSet = pResourceFile->LoadResource(filepath);

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.Parse(pTileSet->GetData().data(), pTileSet->GetData().size());
		if (error != tinyxml2::XML_SUCCESS)
		{
			log.LogError("Filed to get data from Tileset resource file");
			return false;
		}

		tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
		if (strcmp(pRoot->Name(), "tileset") == 0)
		{
			m_tileColums = pRoot->UnsignedAttribute("columns");
			m_tileWidth = pRoot->UnsignedAttribute("tilewidth");
			m_tileHeight = pRoot->UnsignedAttribute("tileheight");
			m_lastgid = pRoot->UnsignedAttribute("tilecount");

			for (tinyxml2::XMLElement* pCurrentElement = pRoot->FirstChildElement(); pCurrentElement;
				pCurrentElement = pCurrentElement->NextSiblingElement())
			{
				if (strcmp(pCurrentElement->Name(), "image") == 0)
				{
					m_imageSource = pCurrentElement->Attribute("source");
					// Normalize path remove "../"
					NormalizeSourcePath();

					auto pImage = pResourceFile->LoadResource(m_imageSource);

					ApplicationLayer::GetInstance()->GetGraphics().PreLoadImage(pImage);
				}
			}
		}
		else
		{
			log.LogError("'tileset' was not found inside the resource file");
			return false;
		}

		return true;
	}

	void TileSet::GetSrc(uint32_t tileIndex, float& srcX, float& srcY)
	{
		uint32_t index = tileIndex - m_lastgid;
		int32_t x = index % m_tileColums;
		int32_t y = index / m_tileColums;
		srcX = (float)(x * m_tileWidth);
		srcY = (float)(y * m_tileHeight);
	}
	void TileSet::NormalizeSourcePath()
	{
		while (m_imageSource[0] == '.')
		{
			size_t pos = m_imageSource.find_first_of("/");
			if (pos != std::string::npos)
				m_imageSource = m_imageSource.substr(pos + 1);
		}
		// replace path to lower case and unify '\\'
		transform(m_imageSource.begin(), m_imageSource.end(), m_imageSource.begin(), ::tolower);
		replace(m_imageSource.begin(), m_imageSource.end(), '\\', '/');
	}
}