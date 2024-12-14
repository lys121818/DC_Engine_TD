#pragma once

// Include from DontCrashEnigne
#include "GameState.h"

// Include from STL
#include <string>
#include <memory>

/*========================================================================

  GameStateMachine - Pure virtual function

========================================================================*/


namespace DC_Engine
{
	class GameStateMachine
	{
	// Functions
	public:
		virtual ~GameStateMachine() {};
		
		virtual bool Init() = 0;
		virtual void Update(float) = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;
		
		virtual void DestroyGameObject(uint32_t id) = 0;
		virtual void ChangeState(std::unique_ptr<GameState>) = 0;

		virtual DC_Engine::GameState& GetCurrentState() { return *m_pCurrentState; }
		virtual std::string GetName() { return m_title; }

	// Variable
	protected:
		std::string m_title = "";
		std::unique_ptr<GameState> m_pCurrentState;
		std::unique_ptr<GameState> m_pNextState;

	};
}