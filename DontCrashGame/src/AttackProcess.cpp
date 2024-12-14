#include "AttackProcess.h"

// Include from DontCrashGame
#include "TransformComponent.h"
#include "TriggerBoxComponent.h"

// Include from DontCrashEngine
#include <GameObject.h>
#include <ApplicationLayer.h>

AttackProcess::AttackProcess(std::weak_ptr<DC_Engine::GameObject> pObject, const float& attackDelay)
	:m_pGameObject(pObject),
    m_delay(attackDelay),
	m_pTransform(nullptr),
    m_pTriggerBox(nullptr)
{
}

AttackProcess::~AttackProcess()
{
}

bool AttackProcess::Init()
{
    auto pObject = m_pGameObject.lock();

    if (!pObject)
    {
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError("AttackProcess: Unable to get game object");
        return false;
    }

    m_pTransform = static_cast<TransformComponent*>(pObject->GetComponent(DC_Engine::kTransformId));

    if (!m_pTransform)
    {
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError("AttackProcess: Unable to get transform from the game object");
        return false;
    }

    m_pTriggerBox = static_cast<TriggerBoxComponent*>(pObject->GetComponent(DC_Engine::kTriggerBoxId));

    if (!m_pTriggerBox)
    {
        DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError("AttackProcess: Unable to get trigger box from the game object");
        return false;
    }

    return true;
}

bool AttackProcess::Update(float deltaTime)
{
    if (m_pGameObject.expired())
    {
        Fail();
        return false;
    }

    if (m_delay <= deltaTime)
    {
        if (m_pTriggerBox->IsInBoundery())
        {
            Succeed();
            return true;
        }
        else
        {
            Fail();
            return false;
        }
    }
    else
    {
        if (m_pTriggerBox->IsInBoundery())
        {
            auto targetRect = m_pTriggerBox->GetTargetRect();
            float angle = m_pTransform->GetRect().position.GetAngle(DC_Engine::Vector2_f(targetRect.position.x, targetRect.position.y));
            m_pTransform->Set_Angle(angle);
        }
    }



    m_delay -= deltaTime;
	return true;
}
