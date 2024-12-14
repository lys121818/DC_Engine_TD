#include "TriggerBoxComponent.h"

// Include from DontCrashEngine
#include <GameObject.h>
#include <ApplicationLayer.h>

//Include from DontCrashGame
#include "TransformComponent.h"

//Include from Tinyxml
#include <tinyxml2.h>

TriggerBoxComponent::TriggerBoxComponent(DC_Engine::GameObject* pOwner, const std::string& name)
	:DC_Engine::DC_ObjectComponent(pOwner,name),
	m_pOwnerTransform(nullptr),
    m_pTargetTransform(nullptr)
{
}


TriggerBoxComponent::~TriggerBoxComponent()
{
}

bool TriggerBoxComponent::Init(tinyxml2::XMLElement* pData)
{
    auto pElement = pData->FirstChildElement("Size");

    // Error Check
    if (pElement == nullptr)
    {
        std::string errorText;
        errorText = "Failed to find trigger Size data from ";
        errorText += "GameObject Id: ";
        errorText += std::to_string(m_pOwner->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        return false;
    }
    else
    {
        m_xBoundery = pElement->FloatAttribute("x");
        m_yBoundery = pElement->FloatAttribute("y");
    }

    return true;
}

bool TriggerBoxComponent::PostInit()
{
    m_pOwnerTransform = static_cast<TransformComponent*>(GetOwner()->GetComponent(DC_Engine::kTransformId));

    if (!m_pOwnerTransform)
    {
        std::string errorText;
        errorText = "TransformComponent has failed to get transform component from Object Id: ";
        errorText += std::to_string(GetOwner()->GetId());
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
    }

	return true;
}

void TriggerBoxComponent::Update(float)
{
    m_pTriggerBoxTrasform.position.x = m_pOwnerTransform->Get_X() - (m_xBoundery / 2);
    m_pTriggerBoxTrasform.position.y = m_pOwnerTransform->Get_Y() - (m_yBoundery / 2);

    m_pTriggerBoxTrasform.width = m_pOwnerTransform->Get_Width() + (int)m_xBoundery;
    m_pTriggerBoxTrasform.height = m_pOwnerTransform->Get_Height() + (int)m_yBoundery;

    
}

// #DG: we'll talk about this when we get to physics, but you don't want to iterate over all the objects to determine which is overlapping
// this is espcicially true if it's possible for more than one object to overlap
std::shared_ptr<DC_Engine::GameObject> TriggerBoxComponent::IsInBoundery()
{
    // no target
    if (m_targetType == DC_Engine::ObjectType::kNone)
        return nullptr;

    auto& objectsToSearch = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();

    for (auto& object : objectsToSearch)
    {
        if (object.second->GetId() == m_pOwner->GetId() || object.second->GetTypeId() == DC_Engine::ObjectType::kNone)
            continue;

        if (object.second->GetTypeId() == m_targetType || m_targetType == DC_Engine::ObjectType::kPlayer)
        {
            auto pTargetTransform = static_cast<TransformComponent*>(object.second->GetComponent(DC_Engine::kTransformId));
            if (CheckForBoundery(pTargetTransform))
            {
                m_pTargetTransform = pTargetTransform;
                return object.second;
            }

        }
    }

    return nullptr;
}

bool TriggerBoxComponent::CheckForBoundery(TransformComponent* transform)
{
    DC_Engine::Rect ownerRect = m_pTriggerBoxTrasform;
    DC_Engine::Rect targetRect = transform->GetRect();

    if (ownerRect.position.x <= (targetRect.position.x + targetRect.width) && 
        ownerRect.position.x + ownerRect.width >= targetRect.position.x &&
        ownerRect.position.y <= (targetRect.position.y + targetRect.height) &&
        ownerRect.position.y + ownerRect.height >= targetRect.position.y)
        return true;

    return false;
}



