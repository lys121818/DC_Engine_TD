#pragma once
#include <DC_View.h>

// Include From STL
#include <memory>
#include <vector>
#include <functional>

// Include From DontCrashGame

// Forward declare
namespace DC_Engine
{
	class GameObject;
	class EngineGraphics;
	class MouseInput;
	class MouseMove;
	class KeyboardInputEvent;
	class DC_Audio;
	class EventManager;
}
class TransformComponent;
class ImageRenderComponent;
class SoundComponent;

class GameView : public DC_Engine::DC_View
{
	static constexpr size_t s_kMaxButtonObjects = 3;
	using ObjectId = uint32_t;
//Functions
public:
	GameView(DC_Engine::DC_View::Id viewType);
	bool Init() override;
	void UpdateInput(float deltaTime) override;
	void ViewScene(DC_Engine::EngineGraphics* pRenderer) override;
	void Destroy() override;
	DC_Engine::DC_View::Id GetTypeId() const override { return m_viewType; }

	void SetGameObject(std::shared_ptr<DC_Engine::GameObject> pButton);
	void SetPosition(float x, float y) const;

	static size_t GetMaxButtonCount() { return s_kMaxButtonObjects; }

private:
	// Init
	void ButtonsInit();

	// Update
	void HandleInput();
	void MouseMoveEvent();
	void MouseInputEvent();
	void KeyboardEvent();
// Variables
private:
	// ViewType
	DC_Engine::DC_View::Id m_viewType;

	DC_Engine::EventManager* m_pEventManger;

	// Object and Components
	std::shared_ptr<DC_Engine::GameObject> m_pGameObject;
	TransformComponent* m_pObjectTransform;
	ImageRenderComponent* m_pObjectImage;
	SoundComponent* m_pObjectSound;

	// Interface controller
	DC_Engine::DC_Audio& m_pAudio;
	DC_Engine::MouseInput& m_pMouseButtonEvent;
	DC_Engine::MouseMove& m_pMouseMoveEvent;
	DC_Engine::KeyboardInputEvent& m_pKeyboardInputEvent;

};

