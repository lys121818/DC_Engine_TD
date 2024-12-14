#include "MoveProcess.h"
#include "TransformComponent.h"

// Include from DontCrashEngine
#include <Log.h>
#include <Vector.h>
#include <GameObject.h>
#include <ApplicationLayer.h>

MoveProcess::MoveProcess(std::weak_ptr<DC_Engine::GameObject> pObject, float time, float speed)
    :m_pGameObject(pObject),
    m_time(time),
    m_speed(speed),
    m_pTransform(nullptr)
{

}

MoveProcess::~MoveProcess()
{
}

bool MoveProcess::Init()
{
    auto pObject = m_pGameObject.lock();
    if (!pObject)
    {
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError("MoveProcess: Unable to get game object");
        return false;
    }

    m_pTransform = static_cast<TransformComponent*>(pObject->GetComponent(DC_Engine::kTransformId));

    if (!m_pTransform)
    {
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError("MoveProcess: Unable to get transform from the game object");
        return false;
    }

    return true;
}

bool MoveProcess::Update(float deltaTime)
{
    if (m_pGameObject.expired())
    {
        Fail();
        return false;
    }

    if (m_time <= deltaTime)
    {
        Succeed();
        return true;
    }
    else
    {
        DC_Engine::Vector2_f directionToMove = DC_Engine::Vector2_f::NormalizeAngle(m_pTransform->GetRect().angle);
        directionToMove.x *= m_speed * deltaTime;
        directionToMove.y *= m_speed * deltaTime;

        // finish move process when it hits the wall
        if (m_pTransform->TryMove(directionToMove.x, directionToMove.y) == false)
        {
            Succeed();
        }
        m_time -= deltaTime;
    }

    return true;
}
