#pragma once
#include "DC_PhysicsManager.h"
#include "box2d.h"
#include "Box2DContactListener.h"
#include <unordered_map>

namespace DC_Engine
{
	class Box2DPhysics : public DC_PhysicsManager
	{
	public:
		static const int32_t s_kSubStepCount = 4;

	// Functions
	public:
		Box2DPhysics(float x, float y);
		virtual ~Box2DPhysics();

		bool Init() override;
		void Update(float deltaTime) override;
		virtual std::unique_ptr<DC_StaticBodyComponent> CreateStaticBodyComponent(GameObject* pOwner, const char* pName) override;
		virtual std::unique_ptr<DC_DynamicBodyComponent> CreateDynamicBodyComponent(GameObject* pOwner, const char* pName) override;
		void RegisterStaticBody(DC_StaticBodyComponent* pComponent) override;
		void UnregisterStaticBody(DC_StaticBodyComponent* pComponent) override;
		void RegisterDynamicBody(DC_DynamicBodyComponent* pComponent) override;
		void UnregisterDynamicBody(DC_DynamicBodyComponent* pComponent) override;
		void SetContactListener(std::shared_ptr<DC_ContactListener> pContactListener) override;
		virtual b2WorldId GetWorldId() override { return m_worldId; }

	// Variables
	private:
		b2WorldId m_worldId;
		std::unordered_map<int32_t, GameObject*> m_shapeToObject;
		std::shared_ptr<DC_ContactListener> m_pContactListener;
		std::unique_ptr<Box2DContactListener> m_pBox2DContactListener;

	};
}

