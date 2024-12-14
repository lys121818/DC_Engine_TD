#pragma once

// Include from DontCrashEngine
#include "DC_ObjectComponent.h"
#include "GameObject.h"

// Include from STL
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace DC_Engine
{
	// ForwardDeclare
	class Resource;
	class ObjectFactory
	{
	// reinitialize name of the data type
	public:
		// Component function that creates the component
		using ComponentFunction = std::function<std::unique_ptr<DC_ObjectComponent>(GameObject*, std::string)>;

	// Functions
	public:
		ObjectFactory() : m_nextObjectId(0) {}
		~ObjectFactory() {}

		std::shared_ptr<GameObject> CreateGameObject(std::shared_ptr<Resource> pResource); // Creates and initialize the object
		inline void RegisterComponentCreator(std::string componentName, ComponentFunction pFunction)
		{ m_componentCreatorMap[componentName] = pFunction;	}

		GameObject::Id GetNextObjectId() { return m_nextObjectId++; }
	private:
		std::unique_ptr<DC_ObjectComponent> CreateComponent(tinyxml2::XMLElement* pData, GameObject* pOwner);

	// Variable
	private:
		GameObject::Id m_nextObjectId;
		std::unordered_map<std::string, ComponentFunction> m_componentCreatorMap;
	};

}