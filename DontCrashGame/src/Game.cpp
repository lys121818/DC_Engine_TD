#include "Game.h"

// include from Engine
#include <ApplicationLayer.h>
#include <input.h>

// include from STL

Game::Game()
{
}

Game::~Game()
{
	Destroy();
}

bool Game::Init()
{
	m_title = "MyGame";
	m_pCurrentState = DC_Engine::GameState::Create(DC_Engine::State::SandBox);
	return m_pCurrentState->Init(); 

}

void Game::Update(float deltaTime)
{
	if (m_pCurrentState == nullptr)
		return;

	// Chage State
	if (m_pNextState != nullptr)
	{
		m_pCurrentState->Exit();
		m_pCurrentState.swap(m_pNextState);
		m_pNextState.reset(nullptr);
	}

	// Update state
	m_pCurrentState->Update(deltaTime);
}

void Game::Render()
{
	if (m_pCurrentState == nullptr)
		return;

	// Render state
	m_pCurrentState->Render();
}

// Safe Destroy
void Game::Destroy()
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit();
}

void Game::DestroyGameObject(uint32_t id)
{
	m_pCurrentState->DestoryGameObject(id);
}

// #DG: does the game ever actually change states? 
// If not this state machine logic is extra complexity that's not needed.
void Game::ChangeState(std::unique_ptr<DC_Engine::GameState> state)
{
	// Change state
	m_pNextState = std::move(state);
	m_pNextState->Init();
}
