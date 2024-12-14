#pragma once
#include "DC_DynamicBodyComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Vector.h"
#include <box2d.h>
namespace DC_Engine
{
	class GameObject;

	class Box2DDynamicBody : public DC_DynamicBodyComponent
	{
	public:
		Box2DDynamicBody(GameObject* pOwner, const char* pName, b2WorldId pWorldId)
			:DC_DynamicBodyComponent(pOwner,pName),
			m_parentWorldId(pWorldId),
			m_bodyId(),
			m_shapeId(),
			m_pTransform(nullptr),
			m_demension({0.0f,0.0f}),
			m_density(0.0f),
			m_friction(0.0f)
		{

		}

		virtual ~Box2DDynamicBody();

		inline virtual void SetLinearVelocity(float x, float y) override
		{
			b2Body_SetLinearVelocity(m_bodyId, b2Vec2(x, y));
		}

		inline virtual void Set_TransformAngle(float angle) override
		{
			m_pTransform->Set_Angle(angle);
		}

		inline virtual Rect Get_TransformRect() override
		{
			return m_pTransform->GetRect();
		}

		inline const b2ShapeId GetShapeId() { return m_shapeId; }

		virtual bool Init(tinyxml2::XMLElement* pData) override;

		virtual bool PostInit() override;

		virtual void Update(float deltaTime) override;

	private:
		b2WorldId m_parentWorldId;
		b2BodyId m_bodyId;
		b2ShapeId m_shapeId;

		TransformComponent* m_pTransform;

		Vector2_f m_demension;
		float m_density;
		float m_friction;

	};
}

