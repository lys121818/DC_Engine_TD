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

class MoveProcess : public DC_Engine::DC_Process
{
	
// Functions
public:
	MoveProcess(std::weak_ptr<DC_Engine::GameObject> pObject,float time, float speed);
	~MoveProcess();
	bool Init() override;
	bool Update(float deltaTime) override;

// Variables
private:
	std::weak_ptr<DC_Engine::GameObject> m_pGameObject;
	TransformComponent* m_pTransform;
	float m_speed;
	float m_time;
};

