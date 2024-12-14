#pragma once
// Include from DontCrashEngine
#include "DC_View.h"
#include "EventManager.h"
#include "ScriptingManager.h"
#include "ProcessManager.h"
#include "ResourceFile.h"
#include "ObjectFactory.h"
#include "DC_PhysicsManager.h"

#include <memory>
#include <unordered_map>

namespace DC_Engine
{

	enum class State
	{
		None = 0,
		SandBox
	};
	
	class GameObject;

	class GameState
	{
	public:
		using ViewId = uint32_t;
		using GameObjects = std::unordered_map<uint32_t, std::shared_ptr<DC_Engine::GameObject>>;
		virtual ~GameState() {};


		virtual bool Init() = 0;
		virtual void Update(float) = 0;
		virtual void Render() = 0;
		virtual void Exit() = 0;
		virtual void AddView(std::unique_ptr<DC_Engine::DC_View> pView, ViewId viewId) = 0;
		virtual void DestoryGameObject(uint32_t id) = 0;
		virtual EventManager& GetEventManager() { return *m_pEventManager; }
		virtual ScriptingManager& GetScriptingManager() { return *m_pScriptingManager; }
		virtual ResourceFile& GetResourceFile() { return *m_pResouceFile; }
		virtual ObjectFactory& GetObjectFactory() { return *m_pObjectFactory; }
		virtual GameObjects& GetGameObjects() { return m_pGameObjects; }
		virtual DC_PhysicsManager& GetPhysicsManager() { return *m_pPhysicsManager; }
		virtual void RegisterWithLua();

		// Creates the correct state according to the given parameter State
		static std::unique_ptr<GameState> Create(State state);

	protected:

		// Views to run
		std::unordered_map<ViewId,std::unique_ptr<DC_View>> m_pViews;
		std::unique_ptr<EventManager> m_pEventManager = nullptr;
		std::unique_ptr<ScriptingManager> m_pScriptingManager = nullptr;
		std::unique_ptr<DC_PhysicsManager> m_pPhysicsManager = nullptr;
		std::unique_ptr<ResourceFile> m_pResouceFile = nullptr;
		std::unique_ptr<ObjectFactory> m_pObjectFactory = nullptr;
		GameObjects m_pGameObjects;


	};
}