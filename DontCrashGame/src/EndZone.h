#pragma once
#include <DC_View.h>
#include <ProcessManager.h>

#include <memory>

namespace DC_Engine
{
	class EventManager;
	class GameObject;
	class DC_Process;
}

class EndZone : public DC_Engine::DC_View
{
public:
	EndZone(DC_Engine::DC_View::Id viewType);
	// Functions
	bool Init() override;
	void UpdateInput(float deltaTime) override;
	void ViewScene(DC_Engine::EngineGraphics* pRenderer) override;

	DC_Engine::DC_View::Id GetTypeId() const override { return m_viewType; }

	void SetGameObject(std::shared_ptr<DC_Engine::GameObject> pObject) { m_pGameObject = pObject; }

private:
	void CreateTriggerBoxProcess();

// Variables
private:
	// Event
	DC_Engine::EventManager* m_pEventManager;

	// Process
	DC_Engine::ProcessManager m_processManager;
	std::shared_ptr<DC_Engine::DC_Process> m_pTriggerBoxProcess;

	// ViewType
	DC_Engine::DC_View::Id m_viewType;

	// Object
	std::shared_ptr<DC_Engine::GameObject> m_pGameObject;
};

