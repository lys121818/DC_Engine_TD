#pragma once
// Include from DontCrashEngine
#include <DC_View.h>
#include <ProcessManager.h>

// Include from STL
#include <vector>
#include <memory>
#include <unordered_map>
// Forward Decalre

namespace DC_Engine
{
	class GameObject;
	class EngineGraphics;
	class DC_Process;
	class EventManager;
	class Event;
}

class TransformComponent;
class ImageRenderComponent;
class TriggerBoxComponent;

// #DG: this and similar classes need better, more descriptive names
// Maybe something like EnemyBehavior or EnemyAgent or EnemyLogic.
// The word "view" seems to have too many meanings in the project
class EnemyView : public DC_Engine::DC_View
{
	static constexpr float s_kMaxSearchingTime = 2.0f;
	static constexpr float s_kMaxMovingTime = 3.0f;
	static constexpr float s_kMaxDelayTime = 3.0f;
	static constexpr float s_kMaxMoveSpeed = 50.0f;
	static constexpr float s_kMaxRotationSpeed = 30.0f;


	using Id = uint32_t;
public:
	EnemyView(DC_Engine::DC_View::Id viewType);

	// Inherited via DC_View
	bool Init() override;
	void UpdateInput(float deltaTime) override;
	void ViewScene(DC_Engine::EngineGraphics* pRenderer) override;
	DC_Engine::DC_View::Id GetTypeId() const override { return m_viewType; }

	void SetEnemy(std::shared_ptr<DC_Engine::GameObject> pEnemyObject);
	void SetTargetType(DC_Engine::GameObject* pTargetObject);


private:
	// Enemy Mechanics
	void CreateLookAroundProcess();
	void CreateMoveProcess();
	void CreateTriggerBoxProcess();
	void CreateAttackProcess();

	void DestroyEvent(DC_Engine::Event* pEvent);

private:
	// ViewType
	DC_Engine::DC_View::Id m_viewType;

	// Processes
	DC_Engine::ProcessManager m_processManager;
	std::shared_ptr<DC_Engine::DC_Process> m_pLookAroundProcess;
	std::shared_ptr<DC_Engine::DC_Process> m_pMoveProcess;
	std::shared_ptr<DC_Engine::DC_Process> m_pTriggerBoxProcess;
	std::shared_ptr<DC_Engine::DC_Process> m_pAttackProcess;
	std::shared_ptr<DC_Engine::DC_Process> m_pDelayProcess;

	// Event
	DC_Engine::EventManager* m_pEventManager;

	// Object and Components
	std::shared_ptr<DC_Engine::GameObject> m_pGameObject;

	// Interface
	std::unique_ptr<DC_Engine::GameObject> m_pTarget;

	size_t m_destroyEventId;





	float time = 0;

};

