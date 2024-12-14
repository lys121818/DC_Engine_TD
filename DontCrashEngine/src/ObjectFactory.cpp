#include "ObjectFactory.h"

// Include from DontCrashEngine
#include "Resource.h"
#include "ApplicationLayer.h"

// Include from TinyXML
#include <tinyxml2.h>


namespace DC_Engine
{

	std::shared_ptr<GameObject> ObjectFactory::CreateGameObject(std::shared_ptr<Resource> pResource)
	{
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError error = doc.Parse(pResource->GetData().data(), pResource->GetData().size());

        if (error != tinyxml2::XML_SUCCESS)
        {
            
            std::string errorText;
            // Set Error text log to print;
            errorText += "Unable to load xml file: ";
            errorText += pResource->GetName();
            errorText += " XMLError Code: ";
            errorText += tinyxml2::XMLDocument::ErrorIDToName(error);
            ApplicationLayer::GetInstance()->GetLog().LogError(errorText, error);
            return nullptr;
        }

        // Create the object
        std::shared_ptr<GameObject> pGameObject = std::make_shared<GameObject>(m_nextObjectId);
        m_nextObjectId++;

        tinyxml2::XMLElement* pRoot = doc.FirstChildElement();

        pGameObject->SetName(pRoot->Attribute("name"));

        // Error Check
        if (!pGameObject->Init(pRoot))
        {
            std::string errorText;
            errorText += "Unable to initialize game object: ";
            errorText += pRoot->Name();

            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return nullptr;
        }

        // Add all components to GameObject
        for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(); 
            pElement;
            pElement = pElement->NextSiblingElement())
        {
            pGameObject->AddComponent(CreateComponent(pElement, pGameObject.get()));
        }

        if (!pGameObject->PostInit())
        {
            std::string errorText;
            errorText += "Unable to post initialize game object: ";
            errorText += pRoot->Name();

            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
            return nullptr;
        }

        return pGameObject;
	}

	std::unique_ptr<DC_ObjectComponent> ObjectFactory::CreateComponent(tinyxml2::XMLElement* pData, GameObject* pOwner)
	{
        std::string pName = pData->Name();

        std::unique_ptr<DC_ObjectComponent> pComponent;

        auto createItr = m_componentCreatorMap.find(pName);

        if (createItr != m_componentCreatorMap.end())
        {
            pComponent = std::move(createItr->second(pOwner, pName));
        }
        // Error Check
        else
        {
            std::string errorText;

            // Set Error text log to print;
            errorText += "Unable to find creator function for component: ";
            errorText += pName;

            ApplicationLayer::GetInstance()->GetLog().LogError(errorText);

            return nullptr;
        }

        if (pComponent != nullptr && !pComponent->Init(pData))
        {
            return nullptr;
        }

        return pComponent;
	}
}