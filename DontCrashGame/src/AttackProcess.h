#pragma once
// Include from DontCrashEngine
#include <DC_Process.h>

//Include from STL
#include <memory>

// Forward decalre
namespace DC_Engine
{
	class GameObject;
}

class TransformComponent;
class TriggerBoxComponent;

class AttackProcess : public DC_Engine::DC_Process
{
	// Functions
public:
	AttackProcess(std::weak_ptr<DC_Engine::GameObject> pObject, const float& attackDelay);
	~AttackProcess();
	bool Init() override;
	bool Update(float deltaTime) override;

	// Variables
private:
	std::weak_ptr<DC_Engine::GameObject> m_pGameObject;
	float m_delay;
	TransformComponent* m_pTransform;
	TriggerBoxComponent* m_pTriggerBox;
};

