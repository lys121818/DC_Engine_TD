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
class LookAroundProcess : public DC_Engine::DC_Process
{
// Functions
public:
	LookAroundProcess(std::weak_ptr<DC_Engine::GameObject> pObject, float time, float rotationSpeed);
	~LookAroundProcess();

	bool Init() override;
	bool Update(float deltaTime) override;

// Variables
private:
	std::weak_ptr<DC_Engine::GameObject> m_pGameObject;
	TransformComponent* m_pTransform;
	float m_time;
	float m_rotationSpeed;
	bool m_direction;
};

