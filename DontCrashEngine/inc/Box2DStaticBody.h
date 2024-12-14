#pragma once
#include "DC_StaticBodyComponent.h"
#include <box2d.h>
#include <Vector.h>
#include "TransformComponent.h"


namespace DC_Engine
{

	class Box2DStaticBody : public DC_StaticBodyComponent
	{
	public:
		Box2DStaticBody(DC_Engine::GameObject* pOwner, const char* pName, b2WorldId pWorldId)
			: DC_StaticBodyComponent(pOwner, pName),
			m_parentWorldId(pWorldId),
			m_bodyId(),
			m_demension(0.0f, 0.0f),
			m_pTransform(nullptr),
			m_shapeId()
		{
		}
		virtual ~Box2DStaticBody();

		virtual bool Init(tinyxml2::XMLElement* pData) override;

		virtual bool PostInit() override;

		virtual void Update(float deltaTime) override;

		inline const b2ShapeId GetShapeId() { return m_shapeId; }

	private:
		b2WorldId m_parentWorldId;
		b2BodyId m_bodyId;
		b2ShapeId m_shapeId;

		DC_Engine::Vector2_f m_demension;

		TransformComponent* m_pTransform;
	};

}