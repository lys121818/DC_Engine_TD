#include "TriggerBoxProcess.h"

// Include from DontCrashEngine
#include <GameObject.h>
#include <DC_Rect.h>

// Include from DontCrashGame
#include "TriggerBoxComponent.h"

TriggerBoxProcess::TriggerBoxProcess(std::weak_ptr<DC_Engine::GameObject> pObject)
    :m_pGameObject(pObject),
    m_pTriggerBox(nullptr)
{
}

TriggerBoxProcess::~TriggerBoxProcess()
{
}

bool TriggerBoxProcess::Init()
{
    auto pObject = m_pGameObject.lock();

    if (!pObject)
        return false;

    m_pTriggerBox = static_cast<TriggerBoxComponent*>(pObject->GetComponent(DC_Engine::kTriggerBoxId));

    return true;
}

bool TriggerBoxProcess::Update(float)
{
    if (m_pGameObject.expired())
    {
        Fail();
        return false;
    }

	if (m_pTriggerBox->IsInBoundery())
		Succeed();

    return true;
}
