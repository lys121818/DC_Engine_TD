#include "EndZone.h"

#include <GameObject.h>
#include <EventManager.h>
#include <ApplicationLayer.h>

#include "TriggerBoxComponent.h"
#include "ImageRenderComponent.h"

#include "TriggerBoxProcess.h"

#include "EndGameEvent.h"

EndZone::EndZone(DC_Engine::DC_View::Id viewType)
    :m_viewType(viewType),
    m_pEventManager(nullptr)
{
    m_pEventManager = &DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetEventManager();
}

bool EndZone::Init()
{
    auto pTriggerBox = static_cast<TriggerBoxComponent*>(m_pGameObject->GetComponent(DC_Engine::kTriggerBoxId));
    pTriggerBox->SetTargetType(DC_Engine::ObjectType::kPlayer);
    CreateTriggerBoxProcess();
    m_processManager.AttachProcess(m_pTriggerBoxProcess);

    auto pImageComponent = static_cast<ImageRenderComponent*>(m_pGameObject->GetComponent(DC_Engine::kImageId));
    pImageComponent->SetState("Zone");


    return true;
}

void EndZone::UpdateInput(float deltaTime)
{
    m_processManager.UpdateProcess(deltaTime);
}

void EndZone::ViewScene(DC_Engine::EngineGraphics* pRenderer)
{
    m_pGameObject->Render(pRenderer);
}

void EndZone::CreateTriggerBoxProcess()
{
	m_pTriggerBoxProcess = std::make_shared<TriggerBoxProcess>(m_pGameObject);
	m_pTriggerBoxProcess->SetSuccessCallback([this]()
		{
            m_pEventManager->QueueEvent(std::make_unique<EndGameEvent>());
		});
}
