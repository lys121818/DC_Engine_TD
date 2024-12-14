#include "GameObject.h"

// Include from DontCrashEngine
#include "ApplicationLayer.h"

// Include from STL
#include <string>


namespace DC_Engine
{
    bool GameObject::Init(tinyxml2::XMLElement* pData)
    {
        if (pData == nullptr)
        {
            std::string errorText;
            errorText = "XML element is empty";
            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

        std::string name = pData->Name();

        if (name != "GameObject")
        {
            std::string errorText;
            errorText = "Data type is not a game object, the type returns: ";
            errorText += name;
            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return false;
        }

        return true;
    }

    bool GameObject::PostInit()
    {
        bool isSuccess = false;
        for (auto& component : m_pComponents)
        {
            isSuccess = component.second->PostInit();

            // Error Check
            if (!isSuccess)
            {
                std::string errorText;
                errorText = "It has failed to post init component: ";
                errorText += std::to_string(component.second->GetId());
                ApplicationLayer::GetInstance()->GetLog().LogError(errorText);

                return isSuccess;
            }
        }

        return isSuccess;
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& component : m_pComponents)
            component.second->Update(deltaTime);
    }

    void GameObject::Render(EngineGraphics* pRenderer)
    {
        for (auto& component : m_pComponents)
            component.second->Render(pRenderer);
    }

    void GameObject::RegisterWithScript()
    {
        for (auto& component : m_pComponents)
        {
            component.second->RegisterWithScript();
        }
    }

    void GameObject::AddComponent(std::unique_ptr<DC_ObjectComponent> pComponent)
    {
        if (pComponent == nullptr)
        {
            std::string errorText;
            errorText = "Failed to Add Component";
            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
        }
        else
        {
            m_pComponents[pComponent->GetId()] = std::move(pComponent);
        }
    }
    DC_ObjectComponent* GameObject::GetComponent(DC_ObjectComponent::Id id)
    {
        auto Iter = m_pComponents.find(id);

        // Error Check
        if (Iter == m_pComponents.end())
        {
            std::string errorText;
            errorText = "Failed to GetComponent: ";
            errorText += std::to_string(id);
            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return nullptr;
        }

        return Iter->second.get();
    }
    void GameObject::Destroy() const
    {
        auto& gameStateMachine = ApplicationLayer::GetInstance()->GetGame();

        gameStateMachine.DestroyGameObject(m_id);
    }
}