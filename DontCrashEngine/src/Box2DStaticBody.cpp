#include "Box2DStaticBody.h"
#include "GameObject.h"
#include "ApplicationLayer.h"



namespace DC_Engine
{
	Box2DStaticBody::~Box2DStaticBody()
	{
		ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetPhysicsManager().UnregisterStaticBody(this);

		if (b2Body_IsValid(m_bodyId))
		{
			b2DestroyBody(m_bodyId);
			b2DestroyShape(m_shapeId);
		}


	}
	bool Box2DStaticBody::Init(tinyxml2::XMLElement* pData)
	{
		auto pElement = pData->FirstChildElement("Dimensions");
		if (pElement != nullptr)
		{
			m_demension.x = pElement->FloatAttribute("width");
			m_demension.y = pElement->FloatAttribute("height");
		}
		else
		{
			auto& log = DC_Engine::ApplicationLayer::GetInstance()->GetLog();
			log.LogError("Unable to find Dimensions in Data");
			return false;
		}


		return true;
	}

	bool Box2DStaticBody::PostInit()
	{
		m_pTransform = static_cast<TransformComponent*>(
			GetOwner()->GetComponent(kTransformId));
		if (m_pTransform == nullptr)
		{
			auto& log = DC_Engine::ApplicationLayer::GetInstance()->GetLog();
			log.LogError("Unable to find TransformComponent of the GameObject");
			return false;
		}

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.position = { m_pTransform->Get_X(), m_pTransform->Get_Y() };

		m_bodyId = b2CreateBody(m_parentWorldId, &bodyDef);

		b2Polygon box;
		box.vertices[0] = { -m_demension.x / 2.0f , -m_demension.y / 2.0f }; // left top
		box.vertices[1] = { m_demension.x / 2.0f , -m_demension.y / 2.0f }; // right top
		box.vertices[2] = { -m_demension.x / 2.0f , m_demension.y / 2.0f }; // left bottom
		box.vertices[3] = { m_demension.x / 2.0f , m_demension.y / 2.0f }; // right bottom
		box.count = 4;

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3f;

		b2ShapeId shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &box);

		m_shapeId = shapeId;

		ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetPhysicsManager().RegisterStaticBody(this);

		return true;
	}

	void Box2DStaticBody::Update(float)
	{
		b2Body_SetTransform(m_bodyId, b2Vec2(
				m_pTransform->Get_X() + m_demension.x / 2.0f,
				m_pTransform->Get_Y() + m_demension.y / 2.0f
			),
				b2Rot(m_pTransform->Get_Angle())
		);
	}

}