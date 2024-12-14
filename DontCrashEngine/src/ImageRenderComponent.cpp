#include "ImageRenderComponent.h"

// Include from DontCrashEngine
#include <GameObject.h>
#include <ApplicationLayer.h>
#include <EngineGraphics.h>

//Include from DontCrashGame
#include "TransformComponent.h"

// Include from Utility
#include <FileSystem.h>

//Include from Tinyxml
#include <tinyxml2.h>

ImageRenderComponent::ImageRenderComponent(DC_Engine::GameObject* pOwner, const std::string& name)
	:DC_Engine::DC_ObjectComponent(pOwner,name),
    m_pTextureTransform(nullptr)
{
}

ImageRenderComponent::~ImageRenderComponent()
{
}

bool ImageRenderComponent::Init(tinyxml2::XMLElement* pData)
{
    // Check if the data exist
    if (!pData)
    {
        std::string errorText;
        errorText = "Failed to load XML data from ImageRenderComponent ";
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
        DC_Engine::Rect frameRect;

        // Set ImageSource
        auto data = pElement->FirstChildElement("ImageSource");
        if (data && data->Name())
        {
            m_imageSource = data->Attribute("fileName");
            YS_Util::FileSystem::NormalizePath(m_imageSource);
        }
        else
        {
            std::string errorText;
            errorText = "Failed to load ImageSource from ";
            errorText += "GameObject Id: ";
            errorText += std::to_string(m_pOwner->GetId());
            DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

        // Set Frame Rect
        data = pElement->FirstChildElement("Frame");
        if (data)
        {
            frameRect.position.x = data->FloatAttribute("x");
            frameRect.position.y = data->FloatAttribute("y");
        }
        else
        {
            std::string errorText;
            errorText = "Failed to load image frame position from ";
            errorText += "GameObject Id: ";
            errorText += std::to_string(m_pOwner->GetId());
            DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

        data = pElement->FirstChildElement("Dimensions");
        if (data)
        {
            frameRect.width = data->IntAttribute("width");
            frameRect.height = data->IntAttribute("height");
        }
        else
        {
            std::string errorText;
            errorText = "Failed to load image frame dimensions from ";
            errorText += "GameObject Id: ";
            errorText += std::to_string(m_pOwner->GetId());
            DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

        m_textureFrame[name] = frameRect;
    }
    return true;
}

bool ImageRenderComponent::PostInit()
{
    m_pTextureTransform = static_cast<TransformComponent*>(GetOwner()->GetComponent(DC_Engine::kTransformId));

    if (!m_pTextureTransform)
    {
        std::string errorText;
        errorText = "ImageRenderComponent has failed to get transform component from Object Id: ";
        errorText += std::to_string(GetOwner()->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        return false;
    }

	return true;
}

void ImageRenderComponent::Render(DC_Engine::EngineGraphics* pRenderer)
{
    pRenderer->StartDraw(m_imageSource, m_textureFrame[m_currentState], m_pTextureTransform->GetRect(), m_pTextureTransform->GetRect().angle);
}
