#include "GameView.h"

// Include from DontCrashEnigne
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <EngineGraphics.h>
#include <DC_Rect.h>
#include <Input.h>
#include <EventManager.h>
#include <DC_Audio.h>

// Include from DontCrashGame
#include "TransformComponent.h"
#include "ImageRenderComponent.h"
#include "SoundComponent.h"
#include "EventActionComponent.h"

#include "ButtonPressedEvent.h"

GameView::GameView(DC_Engine::DC_View::Id viewType)
	:
	m_viewType(viewType),
	m_pEventManger(&DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetEventManager()),
	m_pAudio(DC_Engine::ApplicationLayer::GetInstance()->GetAudio()),
	m_pMouseButtonEvent(DC_Engine::ApplicationLayer::GetInstance()->GetMouseButton()),
	m_pMouseMoveEvent(DC_Engine::ApplicationLayer::GetInstance()->GetMouseMove()),
	m_pKeyboardInputEvent(DC_Engine::ApplicationLayer::GetInstance()->GetKeyboard())
{
}

bool GameView::Init()
{
	ButtonsInit();

	return true;
}

void GameView::UpdateInput(float deltaTime)
{
	HandleInput();

	m_pGameObject->Update(deltaTime);
}

void GameView::ViewScene(DC_Engine::EngineGraphics* pRenderer)
{
	m_pGameObject->Render(pRenderer);
}

void GameView::Destroy()
{
}

void GameView::SetGameObject(std::shared_ptr<DC_Engine::GameObject> pButton)
{
	// Add button
	m_pGameObject = pButton;

	// Add button transform
	TransformComponent* transformComponent = static_cast<TransformComponent*>(pButton->GetComponent(DC_Engine::kTransformId));
	m_pObjectTransform = transformComponent;

	ImageRenderComponent* imageComponent = static_cast<ImageRenderComponent*>(pButton->GetComponent(DC_Engine::kImageId));
	m_pObjectImage = imageComponent;

	SoundComponent* soundComponent = static_cast<SoundComponent*>(pButton->GetComponent(DC_Engine::kSoundId));
	m_pObjectSound = soundComponent;

}

void GameView::SetPosition(float x, float y) const
{
	m_pObjectTransform->Set_X(x);
	m_pObjectTransform->Set_Y(y);
}

void GameView::ButtonsInit()
{
	//Set Image
	m_pObjectImage->SetState("Normal");
}

void GameView::HandleInput()
{   
	if(m_pMouseMoveEvent.IsMouseMoved())
		MouseMoveEvent();

	if(m_pMouseButtonEvent.IsButtonPressed(DC_Engine::MouseInput::kButtonLeft) || m_pMouseButtonEvent.IsButtonPressed(DC_Engine::MouseInput::kButtonRight) ||
		m_pMouseButtonEvent.IsButtonReleased(DC_Engine::MouseInput::kButtonLeft) || m_pMouseButtonEvent.IsButtonReleased(DC_Engine::MouseInput::kButtonRight))
		MouseInputEvent();

	KeyboardEvent();
}

void GameView::MouseMoveEvent()
{
	float x = m_pMouseMoveEvent.GetMousePosition().first;
	float y = m_pMouseMoveEvent.GetMousePosition().second;

	for (size_t i = 0; i < s_kMaxButtonObjects; ++i)
	{
		bool isHovered = m_pObjectTransform->GetRect().IsContained(x, y);
		if (isHovered)
		{
			if (m_pObjectImage->GetState() == "Normal")
			{
				m_pObjectImage->SetState("Hover");
				//m_pObjectSound->PlaySound("Hover");
			}
		}
		else
		{

			m_pObjectImage->SetState("Normal");
		}
	}

}

void GameView::MouseInputEvent()
{
	///LEFT BUTTON
	// Pressed
	if (m_pMouseButtonEvent.IsButtonPressed(DC_Engine::MouseInput::kButtonLeft))
	{
		for (size_t i = 0; i < s_kMaxButtonObjects; ++i)
		{
			if (m_pObjectImage->GetState() == "Hover")
			{
				m_pObjectImage->SetState("Press");
			}
		}
	}

	// Release
	if (m_pMouseButtonEvent.IsButtonReleased(DC_Engine::MouseInput::kButtonLeft))
	{
		for (size_t i = 0; i < s_kMaxButtonObjects; ++i)
		{
			// #DG: values like "Press" and "Hover" should be made into constants to reduce potential errors
			if (m_pObjectImage->GetState() == "Press")
			{
				m_pObjectImage->SetState("Hover");
				//m_pObjectSound->PlaySound("Press");

				auto id = static_cast<EventActionComponent*>(m_pGameObject->GetComponent(DC_Engine::kEventActionId))->GetEventId();

				m_pEventManger->QueueEvent(std::make_unique<ButtonPressedEvent>(id));
			}
		}
	}
}

void GameView::KeyboardEvent()
{
	// Keyboard Press Event
	if (m_pKeyboardInputEvent.IsKeyPressed(DC_Engine::KeyboardInputEvent::kCode_ArrowUp))
		m_pAudio.ControlMasterVolum(10);

	if (m_pKeyboardInputEvent.IsKeyPressed(DC_Engine::KeyboardInputEvent::kCode_ArrowDown))
		m_pAudio.ControlMasterVolum(-10);

	if (m_pKeyboardInputEvent.IsKeyPressed(DC_Engine::KeyboardInputEvent::kCode_P))
		m_pAudio.ToggleMusic();
}
