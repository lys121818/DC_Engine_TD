#pragma once
#include <memory>
#include "WindowManager.h"
#include "GameStateMachine.h"
#include "EngineGraphics.h"
#include "DC_Audio.h"
#include "Log.h"
#include "Random.h"

/*==================================================================================

  ApplicationLayer - Virtual vlass of Application


  ----------------------------------------------------------------------------------
  EngineAPI: The API class
  GameStateMachine: The game class

====================================================================================*/

namespace DC_Engine
{
	enum class ActionResult;

	class ApplicationLayer
	{

	public:
		virtual ~ApplicationLayer() {};
		static ApplicationLayer* GetInstance() { return s_appLayer; }

		virtual std::unique_ptr<GameStateMachine> CreateGameStateMachine() = 0;

		virtual ActionResult Init();
		virtual void Run();
		virtual void Render();
		virtual void Destroy();
		virtual void Exit() { m_isRunning = false; }


		WindowManager& GetWindow() { return *m_pWindow; }
		EngineGraphics& GetGraphics() { return *m_pRenderer; }
		GameStateMachine& GetGame() { return *m_pGame; }
		DC_Audio& GetAudio() { return *m_pAudio; }
		KeyboardInputEvent& GetKeyboard() { return *m_pWindow->GetKeyboard(); }
		MouseInput& GetMouseButton(){ return *m_pWindow->GetMouseInput(); }
		MouseMove& GetMouseMove() { return *m_pWindow->GetMouseMove(); }
		RandomNumberGenerator& GetRNG() { return *m_pRng; }
		Logger& GetLog() { return *m_pLog; }

		static std::string GetBasePath();	// Specificly working in window platform

		bool HasAPI() const;
		bool HasGame() const;
		bool HasRenderer() const;
		bool HasEventSystem() const;
		bool HasAudio() const;
	private:

		void Update(float deltaTime);
		void EndFrame();

		virtual ActionResult CreateWindowAPI();
		virtual ActionResult CreateRenderer();
		virtual ActionResult CreateInputEvent();
		virtual ActionResult CreateAudio();

		void SetLogger(); // Initialize and create logger


	protected:
		static ApplicationLayer* s_appLayer;

		std::unique_ptr<Logger> m_pLog;
		std::unique_ptr<WindowManager> m_pWindow;
		std::unique_ptr<GameStateMachine> m_pGame;
		std::unique_ptr<EngineGraphics> m_pRenderer;
		std::unique_ptr<DC_Audio> m_pAudio;
		std::unique_ptr<RandomNumberGenerator> m_pRng;

	private:
		bool m_isRunning = true;
		struct WindowConfig m_config;

	};
}