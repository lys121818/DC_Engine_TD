#pragma once
// Include from DontCrashEngine
#include <DC_Process.h>


//Include from STL
#include <memory>

//Forward decalre
namespace DC_Engine
{
	class GameObject;
}

class TriggerBoxComponent;

class TriggerBoxProcess : public DC_Engine::DC_Process
{
// Functions
public:
	TriggerBoxProcess(std::weak_ptr<DC_Engine::GameObject> pObject);
	~TriggerBoxProcess();

	bool Init() override;
	bool Update(float) override;

// Variabels
private:
	std::weak_ptr<DC_Engine::GameObject> m_pGameObject;
	TriggerBoxComponent* m_pTriggerBox;
};

