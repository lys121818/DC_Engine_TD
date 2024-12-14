#pragma once
// Include from DontCrashEngine
#include <GameStateMachine.h>

// Include from STL
#include <string>
#include <memory>


/*==================================================================

  Game - My actual game screen which runs functions in the scene
  ------------------------------------------------------------------

  Init - Create and initialize the main(first) state

  Update - Check if next state is avaiable. If it does exit
		   current state and change state with next state.

  ChangeState - Sets the given state to m_pNextState

====================================================================*/

class Game : public DC_Engine::GameStateMachine
{
// Functions
public:
	Game();
	virtual ~Game() override;
	
	bool Init() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Destroy() override;
	void DestroyGameObject(uint32_t id) override;
	void ChangeState(std::unique_ptr<DC_Engine::GameState> state) override;

private:

// Variables
public:

private:


};




