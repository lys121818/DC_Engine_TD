#include "LookAroundProcess.h"
#include "TransformComponent.h"

// Include from DontCrashEngine
#include <ApplicationLayer.h>
#include <Log.h>
#include <Vector.h>
#include <GameObject.h>
#include <Random.h>


LookAroundProcess::LookAroundProcess(std::weak_ptr<DC_Engine::GameObject> pObject, float time, float rotationSpeed)
	:m_pGameObject(pObject),
	m_time(time),
    m_rotationSpeed(rotationSpeed),
    m_direction(false)
{
    m_direction = DC_Engine::ApplicationLayer::GetInstance()->GetRNG().Rand() % 2;
}

LookAroundProcess::~LookAroundProcess()
{
}

bool LookAroundProcess::Init()
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
	return false;
}

bool LookAroundProcess::Update(float deltaTime)
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
        // random direction to look around
        float currentAngle = m_pTransform->GetRect().angle;
        if (m_direction)
        {
            m_pTransform->Set_Angle(currentAngle + (m_rotationSpeed * deltaTime));
        }
        else
        {
            m_pTransform->Set_Angle(currentAngle - (m_rotationSpeed * deltaTime));
        }

        m_time -= deltaTime;
    }

	return true;
}
