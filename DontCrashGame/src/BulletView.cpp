#include "BulletView.h"

// Include from DontCrashEngine
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <GameStateMachine.h>
#include <Vector.h>
#include <DC_Event.h>

// Include from DontCrashGame
#include "TransformComponent.h"
#include "ImageRenderComponent.h"
#include "SoundComponent.h"
#include "TriggerBoxComponent.h"

#include "DestroyGameObjectEvent.h"
#include "PlaySoundEvent.h"

#include "MoveProcess.h"
#include "TriggerBoxProcess.h"

BulletView::BulletView(DC_Engine::DC_View::Id viewType)
	:m_viewType(viewType),
	m_pAudio(nullptr),
	m_listenerId(SIZE_MAX)
{
	m_pAudio = &DC_Engine::ApplicationLayer::GetInstance()->GetAudio();
	m_pEventManager = &DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetEventManager();
}

bool BulletView::Init()
{
	BulletInit();

	//auto soundComponent = static_cast<SoundComponent*>(m_pGameObject->GetComponent(DC_Engine::kSoundId));
	
	// #DG: this string is repeated in multiple places and should be extracted to a member variable or constant
	//soundComponent->PlaySound("Fire");
	m_pEventManager->QueueEvent(std::make_unique<PlaySoundEvent>(m_pGameObject->GetSpawnSound()));

	m_listenerId = m_pEventManager->AddEventListener(
		DestroyGameObjectEvent::kEventType,
		std::bind(&BulletView::DestroyEvent, this, std::placeholders::_1));

	CreateMoveProcess();
	CreateTriggerBoxProcess();
	m_processManager.AttachProcess(m_pMoveProcess);
	m_processManager.AttachProcess(m_pTriggerBoxProcess);

	return true;
}

void BulletView::UpdateInput(float deltaTime)
{
	m_processManager.UpdateProcess(deltaTime);
}

void BulletView::ViewScene(DC_Engine::EngineGraphics* pRenderer)
{
	m_pGameObject->Render(pRenderer);
}

void BulletView::Destroy()
{
}

void BulletView::BulletInit()
{
	auto bulletTriggerbox = static_cast<TriggerBoxComponent*>(m_pGameObject->GetComponent(DC_Engine::kTriggerBoxId));
	bulletTriggerbox->SetTargetType(DC_Engine::ObjectType::kAll);

	auto bulletTransform = static_cast<TransformComponent*>(m_pGameObject->GetComponent(DC_Engine::kTransformId));

	// Error Check
	if (bulletTransform == nullptr)
	{
		std::string errorText;
		errorText = "Unable to find Transform component for bullet";
		DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
	}

	auto bulletImage = static_cast<ImageRenderComponent*>(m_pGameObject->GetComponent(DC_Engine::kImageId));

	// Error Check
	if (bulletImage == nullptr)
	{
		std::string errorText;
		errorText = "Unable to find Image component for bullet";
		DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
	}

	bulletImage->SetState("Fire");

}

void BulletView::DestroyEvent(DC_Engine::Event* pEvent)
{
	auto pDestoryObject = static_cast<DestroyGameObjectEvent*>(pEvent);

	if (pDestoryObject->GetId() == m_pGameObject->GetId())
	{
		DestoryGameObject();
	}
}

void BulletView::DestoryGameObject()
{
	auto& pGame = DC_Engine::ApplicationLayer::GetInstance()->GetGame();
	pGame.GetCurrentState().GetEventManager().RemoveEventListener(DestroyGameObjectEvent::kEventType, m_listenerId);
	

	pGame.DestroyGameObject(m_pGameObject->GetId());
}

void BulletView::CreateMoveProcess()
{
	m_pMoveProcess = std::make_shared<MoveProcess>(m_pGameObject, s_kMaxLifeTime, s_kMaxSpeed);
	m_pMoveProcess->SetSuccessCallback([this]()
		{
			m_pEventManager->QueueEvent(std::make_unique<DestroyGameObjectEvent>(m_pGameObject->GetId()));
		});
}

void BulletView::CreateTriggerBoxProcess()
{
	m_pTriggerBoxProcess = std::make_shared <TriggerBoxProcess>(m_pGameObject);
	m_pTriggerBoxProcess->SetSuccessCallback([this]()
		{
			m_pEventManager->QueueEvent(std::make_unique<DestroyGameObjectEvent>(m_pGameObject->GetId()));

			auto object = static_cast<TriggerBoxComponent*>(m_pGameObject->GetComponent(DC_Engine::kTriggerBoxId))->IsInBoundery();
			if(object->GetTypeId() == DC_Engine::ObjectType::kEnemy)
				m_pEventManager->QueueEvent(std::make_unique<DestroyGameObjectEvent>(object->GetId()));

		});
}

