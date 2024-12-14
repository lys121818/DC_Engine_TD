#include "Box2DDynamicBody.h"

#include "ApplicationLayer.h"
#include "TransformComponent.h"


namespace DC_Engine
{
    Box2DDynamicBody::~Box2DDynamicBody()
    {

        if(ApplicationLayer::GetInstance()->HasGame())
            ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetPhysicsManager().UnregisterDynamicBody(this);

        if (b2Body_IsValid(m_bodyId))
        {
            b2DestroyBody(m_bodyId);
            b2DestroyShape(m_shapeId);
        }

    }
    bool Box2DDynamicBody::Init(tinyxml2::XMLElement* pData)
    {
        auto& log = DC_Engine::ApplicationLayer::GetInstance()->GetLog();

        // Set dimensions
        auto pElement = pData->FirstChildElement("Dimensions");
        if (pElement != nullptr)
        {
            m_demension.x = pElement->FloatAttribute("width");
            m_demension.y = pElement->FloatAttribute("height");
        }
        else
        {
            log.LogError("Unable to find Dimensions in Data file.");
            return false;
        }

        // Set density
        pElement = pData->FirstChildElement("Density");
        if (pElement != nullptr)
        {
            m_density = pElement->FloatAttribute("density");
            
        }
        else
        {
            log.LogError("Unable to find Density in Data file.");
            return false;
        }

        // Set friction
        pElement = pData->FirstChildElement("Friction");
        if (pElement != nullptr)
        {
            m_friction = pElement->FloatAttribute("friction");
        }
        else
        {
            log.LogError("Unable to find Friction in Data file.");
            return false;
        }


        return true;
    }

    bool Box2DDynamicBody::PostInit()
    {
        m_pTransform = reinterpret_cast<TransformComponent*>(GetOwner()->GetComponent(kTransformId));
        
        if (m_pTransform == nullptr)
        {
            auto& log = ApplicationLayer::GetInstance()->GetLog();
            log.LogError("Unable to find transform in GameObject");
            return false;
        }

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;

        bodyDef.position = { m_pTransform->Get_X(), m_pTransform->Get_Y() };

        m_bodyId = b2CreateBody(m_parentWorldId, &bodyDef);


        b2Polygon box;
        box = b2MakeBox(m_demension.x, m_demension.y);

        // Initialize remaining fields explicitly if necessary
        for (int i = 4; i < 8; ++i) {
            box.vertices[i] = { 0.0f, 0.0f };
        }

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = m_density;
        shapeDef.friction = m_friction;

        b2ShapeId shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &box);

        m_shapeId = shapeId;

        ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetPhysicsManager().RegisterDynamicBody(this);

        return true;
    }

    void Box2DDynamicBody::Update(float)
    {
        auto bodyPosition = b2Body_GetPosition(m_bodyId);


        b2Body_SetTransform(m_bodyId, b2Vec2(
                m_pTransform->Get_X() + m_demension.x / 2.0f,
                m_pTransform->Get_Y() + m_demension.y / 2.0f
            ),
                b2Body_GetRotation(m_bodyId)
        );

        m_pTransform->Set_X(bodyPosition.x - m_demension.x / 2.0f);
        m_pTransform->Set_Y(bodyPosition.y - m_demension.y / 2.0f);

    }
}

