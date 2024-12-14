#include "Layer.h"
#include <string>
#include <sstream>

namespace DC_Engine
{
	Layer::Layer()
		:
		m_width(0),
		m_height(0)
	{
	}

	bool Layer::Init(tinyxml2::XMLElement* pElement)
	{

		m_width = pElement->UnsignedAttribute("width");
		m_height = pElement->UnsignedAttribute("height");

		m_tiles.reserve(m_width * m_height);

		// Add tiles id for level
		for (tinyxml2::XMLElement* pLayerElement = pElement->FirstChildElement(); pLayerElement;
			pLayerElement = pLayerElement->NextSiblingElement())
		{
			if (strcmp(pLayerElement->Name(), "data") == 0)
			{
				std::stringstream textStream(pLayerElement->GetText());
				std::string id;

				while (std::getline(textStream, id, ','))
				{
					m_tiles.emplace_back(std::stoul(id));
				}
			}
		}

		return true;
	}


}
