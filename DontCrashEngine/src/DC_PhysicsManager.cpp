#include "DC_PhysicsManager.h"
#include "ApplicationLayer.h"
#include "Box2DPhysics.h"

namespace DC_Engine
{
	std::unique_ptr<DC_PhysicsManager> DC_PhysicsManager::Create(Type type, float xGravity, float yGravity)
	{
		switch (type)
		{
		case DC_Engine::DC_PhysicsManager::Type::kNone:
		{
			auto& log = ApplicationLayer::GetInstance()->GetLog();
			log.LogError("The Physics Type is none");
			return nullptr;
		}
		case DC_Engine::DC_PhysicsManager::Type::kBox2D:
		{
			return std::make_unique<Box2DPhysics>(xGravity,yGravity);
			break;
		}
		default:
			break;
		}

		return nullptr;
	}
}

