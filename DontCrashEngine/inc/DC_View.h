#pragma once
#include <cstdint>

namespace DC_Engine
{
	// Forward declare
	class EngineGraphics;


	class DC_View
	{
	public:
		using Id = uint32_t;
	public:
		virtual ~DC_View() {}

		virtual bool Init() = 0;
		virtual void UpdateInput(float) = 0;
		virtual void ViewScene(DC_Engine::EngineGraphics*) = 0;
		virtual void Destroy() {}
		virtual Id GetTypeId() const = 0;

	};

	extern const DC_View::Id kPlayerViewId;
	extern const DC_View::Id kEnemyViewId;
	extern const DC_View::Id kGameViewId;
	extern const DC_View::Id kBulletViewId;
	extern const DC_View::Id kEndGameId;
}