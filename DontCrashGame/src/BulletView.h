#pragma once
#include <DC_View.h>
#include <ProcessManager.h>

// Include from DontCrashEngine

// Include from STL
#include <memory>

namespace DC_Engine
{
	class DC_Audio;
	class GameObject;
	class Event;
	class EventManager;
	class ProcessManager;
	class DC_Process;
}

// Forward declare
class ImageRenderComponent;

class BulletView : public DC_Engine::DC_View
{
	static constexpr float s_kMaxSpeed = 300.0f;
	static constexpr float s_kMaxLifeTime = 2.0f;
// Functions
public:
	BulletView(DC_Engine::DC_View::Id viewType);
	// Functions
	bool Init() override;
	void UpdateInput(float deltaTime) override;
	void ViewScene(DC_Engine::EngineGraphics* pRenderer) override;
	void Destroy() override;
	DC_Engine::DC_View::Id GetTypeId() const override { return m_viewType; }

	void SetGameObject(std::shared_ptr<DC_Engine::GameObject> pObject) { m_pGameObject = pObject; }

private:
	void BulletInit();
	void DestroyEvent(DC_Engine::Event* pEvent);
	void DestoryGameObject();

	void CreateMoveProcess();
	void CreateTriggerBoxProcess();
// Variables
private:
	// Event
	DC_Engine::EventManager* m_pEventManager;

	// Process
	DC_Engine::ProcessManager m_processManager;
	std::shared_ptr<DC_Engine::DC_Process> m_pTriggerBoxProcess;
	std::shared_ptr<DC_Engine::DC_Process> m_pMoveProcess;


	// ViewType
	DC_Engine::DC_View::Id m_viewType;

	// Interface
	DC_Engine::DC_Audio* m_pAudio;

	// Bullet
	std::shared_ptr<DC_Engine::GameObject> m_pGameObject;

	size_t m_listenerId;
};

