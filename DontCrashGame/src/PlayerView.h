#pragma once
// Include from DontCrashEngine
#include <DC_View.h>
#include <ProcessManager.h>

// Include from STL
#include <memory>
#include <vector>

// Forward Declare from DontCrashEngine
namespace DC_Engine
{
	class GameObject;
	class EngineGraphics;
	class KeyboardInputEvent;
	class MouseMove;
	class MouseInput;
	class DC_Audio;
	class EventManager;
	class DC_DynamicBodyComponent;
}
class TransformComponent;
class ImageRenderComponent;
class DelayProcess;


class PlayerView : public DC_Engine::DC_View
{
	static constexpr float s_kMaxReloadTime = 1.0f;
	static constexpr float s_kMaxSpeed = 200.0f;

public:
// Functions
	PlayerView(DC_Engine::DC_View::Id viewType);
	bool Init() override;
	void UpdateInput(float deltaTime) override;
	void ViewScene(DC_Engine::EngineGraphics* pRenderer) override;
	void Destroy() override;
	DC_Engine::DC_View::Id GetTypeId() const override { return m_viewType; }

	void SetPlayer(std::shared_ptr<DC_Engine::GameObject> pPlayer) { m_pPlayer = pPlayer; }

	DC_Engine::GameObject& GetPlayer() { return *m_pPlayer; }
private:
	// Init
	void PlayerInit();

	//Update
	void HandleInput(float deltaTime);
	void KeyboardInputEvent(float deltaTime);
	void MouseMoveEvent(float deltaTime);
	void MouseButtonEvent(float deltaTime);

	// Process
	void CreateReloadProcess();

	// Logic
	void Move(float deltaTime, int x, int y);
// Variables
public:

private:
	// ViewType
	DC_Engine::DC_View::Id m_viewType;

	// Interface
	DC_Engine::DC_Audio* m_pAudio;
	DC_Engine::KeyboardInputEvent* m_pKeyboardEvent;
	DC_Engine::MouseMove* m_pMouseMoveEvent;
	DC_Engine::MouseInput* m_pMouseButtonEvent;

	// Event
	DC_Engine::EventManager* m_pEventManager;

	// Player
	std::shared_ptr<DC_Engine::GameObject> m_pPlayer;
	DC_Engine::DC_DynamicBodyComponent* m_pPlayerBody = nullptr;
	
	ImageRenderComponent* m_pPlayerImage = nullptr;

	// Process
	DC_Engine::ProcessManager m_processManager;
	std::shared_ptr<DelayProcess> m_pReloadProcess;

	bool m_canFire = true;
};

