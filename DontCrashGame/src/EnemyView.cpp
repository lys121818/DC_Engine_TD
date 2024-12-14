#include "EnemyView.h"

// Include from DontCrashEnigne
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <EngineGraphics.h>
#include <DC_Rect.h>
#include <DC_Event.h>

// Include from DontCrashGame //
// Components
#include "TransformComponent.h"
#include "ImageRenderComponent.h"
#include "SoundComponent.h"
#include "TriggerBoxComponent.h"

// Processes
#include "LookAroundProcess.h"
#include "MoveProcess.h"
#include "TriggerBoxProcess.h"
#include "AttackProcess.h"
#include "DelayProcess.h"

// Event
#include "SpawnBulletEvent.h"
#include "PlaySoundEvent.h"
#include "DestroyGameObjectEvent.h"

EnemyView::EnemyView(DC_Engine::DC_View::Id viewType)
	:
	m_viewType(viewType),
	m_pEventManager(nullptr),
	m_destroyEventId(SIZE_MAX)
{
}

bool EnemyView::Init()
{
	m_pEventManager = &DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetEventManager();

	m_destroyEventId = m_pEventManager->AddEventListener(
		DestroyGameObjectEvent::kEventType,
		std::bind(&EnemyView::DestroyEvent, this, std::placeholders::_1));

	auto pTransform = static_cast<TransformComponent*>(m_pGameObject->GetComponent(DC_Engine::kTransformId));

	// Exclude processes from the objects without transform
	if (!pTransform)
		return false;

	CreateLookAroundProcess();
	CreateTriggerBoxProcess();
	m_processManager.AttachProcess(m_pLookAroundProcess);
	m_processManager.AttachProcess(m_pTriggerBoxProcess);
    return true;
}

void EnemyView::UpdateInput(float deltaTime)
{
	m_pGameObject->Update(deltaTime);

	m_processManager.UpdateProcess(deltaTime);

}

void EnemyView::ViewScene(DC_Engine::EngineGraphics* pRenderer)
{
	
	m_pGameObject->Render(pRenderer);
}


void EnemyView::SetEnemy(std::shared_ptr<DC_Engine::GameObject> pEnemyObject)
{
	// Set Enemy
	m_pGameObject = pEnemyObject;

	// transform
	ImageRenderComponent* imageComponent = static_cast<ImageRenderComponent*>(pEnemyObject->GetComponent(DC_Engine::kImageId));
	imageComponent->SetState("Idle");
}

void EnemyView::SetTargetType(DC_Engine::GameObject* pTargetObject)
{
	static_cast<TriggerBoxComponent*>(m_pGameObject->GetComponent(DC_Engine::kTriggerBoxId))->SetTargetType(pTargetObject->GetTypeId());
}

void EnemyView::CreateLookAroundProcess()
{
	m_pLookAroundProcess = std::make_shared<LookAroundProcess>(m_pGameObject, s_kMaxSearchingTime, s_kMaxRotationSpeed);
	m_pLookAroundProcess->SetSuccessCallback([this]()
		{
			CreateMoveProcess();
			m_pLookAroundProcess->AttachChild(m_pMoveProcess);
		});
}

void EnemyView::CreateMoveProcess()
{
	m_pMoveProcess = std::make_shared<MoveProcess>(m_pGameObject, s_kMaxMovingTime, s_kMaxMoveSpeed);
	m_pMoveProcess->SetSuccessCallback([this]()
		{
			CreateLookAroundProcess();
			m_pMoveProcess->AttachChild(m_pLookAroundProcess);
		});
}

void EnemyView::CreateTriggerBoxProcess()
{
	m_pTriggerBoxProcess = std::make_shared<TriggerBoxProcess>(m_pGameObject);
	m_pTriggerBoxProcess->SetSuccessCallback([this]()
		{
			if(m_pMoveProcess)
				m_pMoveProcess->Paused();

			m_pLookAroundProcess->Paused();

			// Set to Attack processes
			CreateAttackProcess();
			m_pTriggerBoxProcess->AttachChild(m_pAttackProcess);
		});
}

// target is in range = attack
// target is out of range = again for searching
void EnemyView::CreateAttackProcess()
{
	m_pAttackProcess = std::make_shared<AttackProcess>(m_pGameObject, s_kMaxDelayTime);
	m_pAttackProcess->SetSuccessCallback([this]()
		{
			auto transform = static_cast<TransformComponent*>(m_pGameObject->GetComponent(DC_Engine::kTransformId));

			m_pEventManager->QueueEvent(std::make_unique<SpawnBulletEvent>("GameObjects/Bullet.xml", transform->GetRect()));
			//Attack
			CreateTriggerBoxProcess();
			m_pAttackProcess->AttachChild(m_pTriggerBoxProcess);
		});
	m_pAttackProcess->SetFailedCallback([this]()
		{
			if(m_pMoveProcess)
				m_pMoveProcess->Resume();

			m_pLookAroundProcess->Resume();

			CreateTriggerBoxProcess();
			m_pAttackProcess->AttachChild(m_pTriggerBoxProcess);
		});
}

void EnemyView::DestroyEvent(DC_Engine::Event* pEvent)
{
	auto pDestoryObject = static_cast<DestroyGameObjectEvent*>(pEvent);

	if (pDestoryObject->GetId() == m_pGameObject->GetId())
	{
		auto& pGame = DC_Engine::ApplicationLayer::GetInstance()->GetGame();
		pGame.GetCurrentState().GetEventManager().RemoveEventListener(DestroyGameObjectEvent::kEventType, m_destroyEventId);

		m_pEventManager->QueueEvent(std::make_unique<PlaySoundEvent>("Audios/EnemyDead.wav"));

		pGame.DestroyGameObject(m_pGameObject->GetId());
	}
}

