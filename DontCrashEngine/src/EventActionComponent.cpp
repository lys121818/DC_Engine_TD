#include "EventActionComponent.h"

// Include from DontCraashEngine
#include <ApplicationLayer.h>
#include <GameObject.h>

// Include from STL
#include <cstddef>

EventActionComponent::EventActionComponent(DC_Engine::GameObject* pOwner, const std::string& name)
	:DC_ObjectComponent(pOwner,name),
    m_eventId(UINT32_MAX)
{
}

bool EventActionComponent::Init(tinyxml2::XMLElement* pData)
{
    // Check if the data exist
    if (!pData)
    {
        std::string errorText;
        errorText = "Failed to load XML data from SoundComponent ";
        errorText += "GameObject Id: ";
        errorText += std::to_string(m_pOwner->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        return false;
    }

    // add all the images to UO_map
    for (tinyxml2::XMLElement* pElement = pData->FirstChildElement();
        pElement;
        pElement = pElement->NextSiblingElement())
    {
        m_eventId = pElement->IntAttribute("id");
        
    }
    return true;

}
