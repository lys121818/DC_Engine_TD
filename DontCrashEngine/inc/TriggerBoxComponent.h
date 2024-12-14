#pragma once
// Include from DontCrashEngine
#include <DC_ObjectComponent.h>
#include <DC_Rect.h>

// Include from DontCrashGame
#include "TransformComponent.h"

// Include from STL
#include <vector>
#include <memory>

// Forward Decalre
namespace DC_Engine
{
	class GameObject;
	enum class ObjectType;
}

class TriggerBoxComponent : public DC_Engine::DC_ObjectComponent
{
public:
	TriggerBoxComponent(DC_Engine::GameObject* pOwner, const std::string& name);
	~TriggerBoxComponent();

	bool Init(tinyxml2::XMLElement* pData) override;
	bool PostInit() override;
	virtual void Update(float) override;

	std::shared_ptr<DC_Engine::GameObject> IsInBoundery();

	void SetTargetType(DC_Engine::ObjectType type) { m_targetType = type; }

	DC_Engine::Rect GetTargetRect() { return m_pTargetTransform->GetRect(); }

	DC_Engine::Rect GetRect() const { return m_pTriggerBoxTrasform; }

private:
	bool CheckForBoundery(TransformComponent* transform);

private:
	DC_Engine::Rect m_pTriggerBoxTrasform = {0.0, 0.0, 0, 0};
	TransformComponent* m_pOwnerTransform;
	DC_Engine::ObjectType m_targetType;
	TransformComponent* m_pTargetTransform;

	float m_xBoundery = 0.0;
	float m_yBoundery = 0.0;
};

