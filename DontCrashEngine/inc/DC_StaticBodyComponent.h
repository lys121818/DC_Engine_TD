#pragma once
#include <DC_ObjectComponent.h>

namespace DC_Engine
{
	class DC_StaticBodyComponent : public DC_Engine::DC_ObjectComponent
	{
	public:
		DC_StaticBodyComponent(DC_Engine::GameObject* pOwner, const std::string& name)
			:DC_Engine::DC_ObjectComponent(pOwner, name)
		{
		}
		virtual ~DC_StaticBodyComponent() {}
	};
}
