#include "PlayerView.h"

// Include from DontCrashEngine
#include <ApplicationLayer.h>
#include <EngineGraphics.h>
#include <Input.h>
#include <GameObject.h>
#include <Vector.h>
#include <GameState.h>

// Include from DontCrashGame
#include "TransformComponent.h"
#include "ImageRenderComponent.h"

#include "DelayProcess.h"

#include "SpawnBulletEvent.h"


PlayerView::PlayerView(DC_Engine::DC_View::Id viewType)
	:
	m_viewType(viewType),
	m_pAudio(&DC_Engine::ApplicationLayer::GetInstance()->GetAudio()),
	m_pKeyboardEvent(&DC_Engine::ApplicationLayer::GetInstance()->GetKeyboard()),
	m_pMouseButtonEvent(&DC_Engine::ApplicationLayer::GetInstance()->GetMouseButton()),
	m_pMouseMoveEvent(&DC_Engine::ApplicationLayer::GetInstance()->GetMouseMove()),
	m_pEventManager(&DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetEventManager())
{
}

bool PlayerView::Init()
{
	PlayerInit();

	return true;
}

void PlayerView::UpdateInput(float deltaTime)
{
	HandleInput(deltaTime);



	m_pPlayer->Update(deltaTime);

	m_processManager.UpdateProcess(deltaTime);
}

void PlayerView::ViewScene(DC_Engine::EngineGraphics* pRenderer)
{
	m_pPlayer->Render(pRenderer);

}
void PlayerView::Destroy()
{
}

void PlayerView::PlayerInit()
{
	m_pPlayerBody = static_cast<DC_Engine::DC_DynamicBodyComponent*>(m_pPlayer->GetComponent(DC_Engine::kDynamicBodyId));

	// Error Check
	if (m_pPlayerBody == nullptr)
	{
		std::string errorText;
		errorText = "Unable to find Dynamic body component for Player";
		DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
	}

	m_pPlayerImage = static_cast<ImageRenderComponent*>(m_pPlayer->GetComponent(DC_Engine::kImageId));

	// Error Check
	if (m_pPlayerImage == nullptr)
	{
		std::string errorText;
		errorText = "Unable to find Image component for Player";
		DC_Engine::ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
	}

	m_pPlayerImage->SetState("Idle");
}

void PlayerView::HandleInput(float deltaTime)
{
	KeyboardInputEvent(deltaTime);
	MouseMoveEvent(deltaTime);
	MouseButtonEvent(deltaTime);
}

void PlayerView::KeyboardInputEvent(float deltaTime)
{
	if (m_pKeyboardEvent->IsKeyDown(DC_Engine::KeyboardInputEvent::kCode_W))
	{
		Move(deltaTime, 0, -10);
	}
	if (m_pKeyboardEvent->IsKeyDown(DC_Engine::KeyboardInputEvent::kCode_D))
	{
		Move(deltaTime, 10, 0);
	}

	if (m_pKeyboardEvent->IsKeyDown(DC_Engine::KeyboardInputEvent::kCode_A))
	{
		Move(deltaTime, -10, 0);
	}

	if (m_pKeyboardEvent->IsKeyDown(DC_Engine::KeyboardInputEvent::kCode_S))
	{
		Move(deltaTime, 0, 10);
	}
}

void PlayerView::MouseMoveEvent(float deltaTime)
{
	deltaTime;
	if (m_pMouseMoveEvent->IsMouseMoved())
	{
		float angle = m_pPlayerBody->Get_TransformRect().position.GetAngle(DC_Engine::Vector2_f(m_pMouseMoveEvent->GetMousePosition().first, m_pMouseMoveEvent->GetMousePosition().second));
		m_pPlayerBody->Set_TransformAngle(angle);
	}
}

void PlayerView::MouseButtonEvent(float deltaTime)
{
	deltaTime;
	if (m_pMouseButtonEvent->IsButtonPressed(DC_Engine::MouseInput::kButtonLeft))
	{
		if (m_canFire)
		{
			m_pEventManager->QueueEvent(std::make_unique<SpawnBulletEvent>("GameObjects/Bullet.xml", m_pPlayerBody->Get_TransformRect()));
			m_canFire = false;
			CreateReloadProcess();
			m_processManager.AttachProcess(m_pReloadProcess);
		}
	}
}

void PlayerView::CreateReloadProcess()
{
	m_pReloadProcess = std::make_shared<DelayProcess>(s_kMaxReloadTime);
	m_pReloadProcess->SetSuccessCallback([this]()
		{
			m_canFire = true;
		});
}

void PlayerView::Move(float deltaTime, int x, int y)
{
	auto& pGameLayer = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
	auto& scripting = pGameLayer.GetScriptingManager();

	auto dynamicBody = static_cast<DC_Engine::DC_DynamicBodyComponent*>(m_pPlayer->GetComponent(DC_Engine::kDynamicBodyId));
	dynamicBody->SetLinearVelocity((float)x, (float)y);

	// Move
	scripting.StartFunction("PlayerMove");
	scripting.PushNumber(deltaTime * x);
	scripting.PushNumber(deltaTime * y);
	scripting.CallFunction(0);
}
