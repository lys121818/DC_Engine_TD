#pragma once

// Include from STL
#include <unordered_map>
#include <memory>
#include <string>

// Include from TinyXML
#include <tinyxml2.h>

namespace DC_Engine
{

	// Forward declare
	class GameObject;
	class EngineGraphics;


	class DC_ObjectComponent
	{
	// reinitialize name of the data type
	public:
	using Id = uint32_t;

	// Functions
	public:
		DC_ObjectComponent(GameObject* pOwner, std::string name);
		virtual ~DC_ObjectComponent() {}

		virtual bool Init(tinyxml2::XMLElement* pData) = 0;
		virtual bool PostInit() { return true; }
		virtual void Update(float) {}
		virtual void Render(EngineGraphics*) {}

		virtual void RegisterWithScript() {}

		Id GetId() const { return m_id; }
		GameObject* GetOwner() { return m_pOwner; }

	private:

	// Variables
	public:
	protected:
		GameObject* m_pOwner;
		Id m_id;
	};

	extern const DC_ObjectComponent::Id kTransformId;
	extern const DC_ObjectComponent::Id kImageId;
	extern const DC_ObjectComponent::Id kSoundId;
	extern const DC_ObjectComponent::Id kTriggerBoxId;
	extern const DC_ObjectComponent::Id kEventActionId;
	extern const DC_ObjectComponent::Id kDynamicBodyId;
	extern const DC_ObjectComponent::Id kStaticBodyId;
	extern const DC_ObjectComponent::Id kTileId;
}