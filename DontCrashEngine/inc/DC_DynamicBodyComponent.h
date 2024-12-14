#pragma once
#include <DC_ObjectComponent.h>

namespace DC_Engine
{
	class GameObject;
	struct Rect;

	class DC_DynamicBodyComponent : public DC_ObjectComponent
	{
	public:
		DC_DynamicBodyComponent(GameObject* pOwner, const std::string& name)
			: DC_ObjectComponent(pOwner, name)
		{
		}
		virtual ~DC_DynamicBodyComponent() { }
	
		virtual void SetLinearVelocity(float x, float y) = 0;

		virtual void Set_TransformAngle(float angle) = 0;

		virtual Rect Get_TransformRect() = 0;
	};
}

