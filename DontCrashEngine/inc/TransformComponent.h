#pragma once
#include "DC_ObjectComponent.h"
#include "ScriptingManager.h"
#include "DC_Rect.h"

using Rect = DC_Engine::Rect;

class TransformComponent : public DC_Engine::DC_ObjectComponent
{
// Functions
public:
	TransformComponent(DC_Engine::GameObject* pOwner, std::string name);
	~TransformComponent();
	// Inherited via DC_ObjectComponent
	bool Init(tinyxml2::XMLElement* pData) override;

	LUA_REGISTER();

	LUA_SETGLUE_NESTED(Set_X, float, x, transform.position);

	LUA_SETGLUE_NESTED(Set_Y, float, y, transform.position);

	LUA_SETGLUE_NESTED(Set_Width, int, width, transform);
	LUA_SETGLUE_NESTED(Set_Height, int, height, transform);

	LUA_SETGLUE_NESTED(Set_Angle, float, angle, transform);

	LUA_GETGLUE_NESTED(Get_X, float, x, transform.position);
	LUA_GETGLUE_NESTED(Get_Y, float, y, transform.position);

	LUA_GETGLUE_NESTED(Get_Width, int, width, transform);
	LUA_GETGLUE_NESTED(Get_Height, int, height, transform);

	LUA_GETGLUE_NESTED(Get_Angle, float, angle, transform);

	DC_Engine::Rect GetRect() const { return m_transform; }

	LUA_TWO_PARAMETER_FUNCTIONGLUE(bool, TryMove, float, x, float, y);
// Variable
public:

private:
	Rect m_transform = { 0.0, 0.0, 0, 0 };
};


