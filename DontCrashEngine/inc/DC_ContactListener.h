#pragma once

namespace DC_Engine
{
	class GameObject;
	class DC_ContactListener
	{
	public:
		virtual void BeginGameObjectTouch(GameObject* pObjectA, GameObject* pObjectB) = 0;
		virtual void EndGameObjectTouch(GameObject* pObjectA, GameObject* pObjectB) = 0;

	};
}
