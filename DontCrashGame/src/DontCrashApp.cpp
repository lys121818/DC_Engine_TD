#include "DontCrashApp.h"

// include from standard library
#include <assert.h>

// include from game
#include "Game.h"


DontCrashApp::~DontCrashApp()
{
}
std::unique_ptr<DC_Engine::GameStateMachine> DontCrashApp::CreateGameStateMachine()
{
    return std::make_unique<Game>();
}