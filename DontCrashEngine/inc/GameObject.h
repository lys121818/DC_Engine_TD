#pragma once
// Include from DontCrashEngine
#include "DC_ObjectComponent.h"

// Include from TinyXML
#include <tinyxml2.h>

// Include from STL
#include <memory>
#include <unordered_map>
#include <functional>

namespace DC_Engine
{
	// Forward declare
	class EngineGraphics;

	enum class ObjectType
	{
		kNone,
		kPlayer,
		kEnemy,
		kNeutral,
		kAll
	};

	class GameObject
	{
	// reinitialize name of the data type
	public:
		using Id = uint32_t;


	// Functions
	public:
		GameObject(Id id) : m_id(id),m_type(ObjectType::kNone) {}
		~GameObject() {};
		bool Init(tinyxml2::XMLElement* pData);
		bool PostInit();
		void Update(float deltaTime);
		void Render(EngineGraphics* pRenderer);

		void RegisterWithScript();

		void AddComponent(std::unique_ptr<DC_ObjectComponent> pComponent);
		DC_ObjectComponent* GetComponent(DC_ObjectComponent::Id id);

		void SetName(std::string name) { m_name = name; }
		void SetType(ObjectType type) { m_type = type; }
		void SetDestroySound(std::string soundName) { m_destroySound = soundName; }
		void SetSpawnSound(std::string soundName) { m_spawnSound = soundName; }

		std::string GetName() const { return m_name; }
		Id GetId() const { return m_id; }
		ObjectType GetTypeId() const { return m_type; }
		std::string GetDestorySound() { return m_destroySound; }
		std::string GetSpawnSound() { return m_spawnSound; }

		void Destroy() const;

	// Variables
	protected:
		Id m_id;
		std::unordered_map< DC_ObjectComponent::Id, 
							std::unique_ptr<DC_ObjectComponent> > m_pComponents;
		std::string m_name;
		std::string m_destroySound;
		std::string m_spawnSound;
		ObjectType m_type;
	};
}