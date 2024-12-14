#include "SoundComponent.h"

// Include from DontCraashEngine
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <DC_Audio.h>
#include <Resource.h>


SoundComponent::SoundComponent(DC_Engine::GameObject* pOwner, const std::string& name)
    :DC_ObjectComponent(pOwner,name),
    m_pAudio(DC_Engine::ApplicationLayer::GetInstance()->GetAudio())
{
}

SoundComponent::~SoundComponent()
{
}

bool SoundComponent::Init(tinyxml2::XMLElement* pData)
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
        std::string name = pElement->Attribute("name");
        std::string source;

        // Set ImageSource
        auto data = pElement->FirstChildElement("SoundSource");
        if (data && data->Name())
        {
            source = data->Attribute("fileName");
            m_soundSource[name] = source;

            if (name == "Destroy")
            {
                m_pOwner->SetDestroySound(source);
            }
            else if (name == "Spawn")
            {
                m_pOwner->SetSpawnSound(source);
            }
        }
        else
        {
            std::string errorText;
            errorText = "Failed to load SoundSource from ";
            errorText += "GameObject Id: ";
            errorText += std::to_string(m_pOwner->GetId());
            DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

    }
    return true;
}


void SoundComponent::PlaySound(std::shared_ptr<DC_Engine::Resource> pResource)
{
    m_pAudio.SoundPlay(pResource);
}
