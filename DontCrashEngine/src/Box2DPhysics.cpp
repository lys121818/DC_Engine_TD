#include "Box2DPhysics.h"
#include "ApplicationLayer.h"
#include "Box2DStaticBody.h"
#include "Box2DDynamicBody.h"

namespace DC_Engine
{
    Box2DPhysics::Box2DPhysics(float x, float y)
    {
        b2WorldDef world = b2DefaultWorldDef();
        world.gravity = { x, y };

        m_worldId = b2CreateWorld(&world);

        if (!b2World_IsValid(m_worldId))
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to create the b2World");
        }
    }

    Box2DPhysics::~Box2DPhysics()
    {
        if (b2World_IsValid(m_worldId))
            b2DestroyWorld(m_worldId);
    }

    bool DC_Engine::Box2DPhysics::Init()
    {
        if (!b2World_IsValid(m_worldId))
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to initialize the b2World");
            return false;
        }

        return true;
    }

    void DC_Engine::Box2DPhysics::Update(float deltaTime)
    {
        b2World_Step(m_worldId, deltaTime, s_kSubStepCount);
    }

    std::unique_ptr<DC_StaticBodyComponent> Box2DPhysics::CreateStaticBodyComponent(GameObject* pOwner, const char* pName)
    {

        return std::make_unique<Box2DStaticBody>(pOwner,pName,m_worldId);
    }

    std::unique_ptr<DC_DynamicBodyComponent> Box2DPhysics::CreateDynamicBodyComponent(GameObject* pOwner, const char* pName)
    {
        return std::make_unique<Box2DDynamicBody>(pOwner, pName, m_worldId);
    }

    void Box2DPhysics::RegisterStaticBody(DC_StaticBodyComponent* pComponent)
    {
        auto pBox2DComponent = static_cast<Box2DStaticBody*>(pComponent);
        m_shapeToObject[pBox2DComponent->GetShapeId().index1] = pBox2DComponent->GetOwner();
    }

    void Box2DPhysics::UnregisterStaticBody(DC_StaticBodyComponent* pComponent)
    {
        auto pBox2DComponent = static_cast<Box2DStaticBody*>(pComponent);
        m_shapeToObject.erase(pBox2DComponent->GetShapeId().index1);
    }

    void Box2DPhysics::RegisterDynamicBody(DC_DynamicBodyComponent* pComponent)
    {
        auto pBox2DComponent = static_cast<Box2DDynamicBody*>(pComponent);
        m_shapeToObject[pBox2DComponent->GetShapeId().index1] = pBox2DComponent->GetOwner();
    }

    void Box2DPhysics::UnregisterDynamicBody(DC_DynamicBodyComponent* pComponent)
    {
        auto pBox2DComponent = static_cast<Box2DDynamicBody*>(pComponent);
        m_shapeToObject.erase(pBox2DComponent->GetShapeId().index1);
    }

    void Box2DPhysics::SetContactListener(std::shared_ptr<DC_ContactListener> pContactListener)
    {
        m_pContactListener = pContactListener;
    }
}

