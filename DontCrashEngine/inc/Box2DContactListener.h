#pragma once
#include "DC_ContactListener.h"
namespace DC_Engine
{
	class Box2DContactListener : public DC_ContactListener
	{
		void BeginGameObjectTouch(GameObject* pObjectA, GameObject* pObjectB) override;
		void EndGameObjectTouch(GameObject* pObjectA, GameObject* pObjectB) override;
	};
}

