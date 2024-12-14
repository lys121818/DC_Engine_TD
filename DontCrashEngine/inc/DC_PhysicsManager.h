#pragma once

#include "DC_StaticBodyComponent.h"
#include "DC_DynamicBodyComponent.h"

// Include from STL
#include <memory>

struct b2WorldId;

namespace DC_Engine
{
	class GameObject;
	class DC_ContactListener;

	class DC_PhysicsManager
	{
	public:
		enum class Type
		{
			kNone = 0,
			kBox2D
		};

	public:
		virtual ~DC_PhysicsManager() = default;
		virtual bool Init() = 0;
		virtual void Update(float deltaTime) = 0;

		virtual std::unique_ptr<DC_StaticBodyComponent> CreateStaticBodyComponent(GameObject* pOwner, const char* pName) = 0;
		virtual std::unique_ptr<DC_DynamicBodyComponent> CreateDynamicBodyComponent(GameObject* pOwner, const char* pName) = 0;

		virtual void RegisterStaticBody(DC_StaticBodyComponent* pComponent) = 0;
		virtual void UnregisterStaticBody(DC_StaticBodyComponent* pComponent) = 0;
		virtual void RegisterDynamicBody(DC_DynamicBodyComponent* pComponent) = 0;
		virtual void UnregisterDynamicBody(DC_DynamicBodyComponent* pComponent) = 0;

		virtual void SetContactListener(std::shared_ptr<DC_ContactListener> pContactListener) = 0;

		virtual b2WorldId GetWorldId() = 0;

		static std::unique_ptr<DC_PhysicsManager> Create(Type type, float xGravity, float yGravity);
	};
}

