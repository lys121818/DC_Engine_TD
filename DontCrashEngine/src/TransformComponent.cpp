// Include from DontCrashEngine
#include <TransformComponent.h>
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <Vector.h>
#include <WindowConfig.h>
#include <WindowManager.h>
#include <GameState.h>
#include <ScriptingManager.h>
#include "TransformComponent.gen"

// Include from Tinyxml
#include <tinyxml2.h>

// Generated with lua

TransformComponent::TransformComponent(DC_Engine::GameObject* pOwner, std::string name)
    :
    DC_Engine::DC_ObjectComponent(pOwner, name),
    m_transform(0,0,0,0)
{
}
TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Init(tinyxml2::XMLElement* pData)
{
    auto pElement = pData->FirstChildElement("Position");

    // Error Check
    if (pElement == nullptr)
    {
        std::string errorText;
        errorText = "Failed to find position data from ";
        errorText += "GameObject Id: ";
        errorText += std::to_string(m_pOwner->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        return false;
    }
    else
    {
        m_transform.position.x = pElement->FloatAttribute("x");
        m_transform.position.y = pElement->FloatAttribute("y");
    }

    pElement = pData->FirstChildElement("Dimensions");
    if (pElement == nullptr)
    {
        std::string errorText;
        errorText = "Failed to find dimensions data from ";
        errorText += "GameObject Id: ";
        errorText += std::to_string(m_pOwner->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        return false;
    }
    else
    {
        m_transform.width = pElement->IntAttribute("width");
        m_transform.height = pElement->IntAttribute("height");
    }

    return true;
}

bool TransformComponent::TryMove(float x, float y)
{
    bool isMovedSuccess = true;

    DC_Engine::Vector2_f nextFramePosition;

    nextFramePosition.x = m_transform.position.x + x;
    nextFramePosition.y = m_transform.position.y + y;

    DC_Engine::Vector2 windowSize = { (int)DC_Engine::ApplicationLayer::GetInstance()->GetWindow().GetWindowInfo().width,
        (int)DC_Engine::ApplicationLayer::GetInstance()->GetWindow().GetWindowInfo().height };

    // Check for valid movement
    if (nextFramePosition.x >= 0 && (nextFramePosition.x + m_transform.width) <= windowSize.x)
    {
        m_transform.position.x = nextFramePosition.x;
    }
    else
    {
        isMovedSuccess &= false;
    }

    if (nextFramePosition.y >= 0 && (nextFramePosition.y + m_transform.height) <= windowSize.y)
    {
        m_transform.position.y = nextFramePosition.y;
    }
    else
    {
        isMovedSuccess &= false;
    }

    return isMovedSuccess;
}
