#pragma once
#include <ApplicationLayer.h>

/*==================================================================================

  DontCrashApp - My Game Engine Class

  This is the Game Engine which runs the game with the API

====================================================================================*/


class DontCrashApp : public DC_Engine::ApplicationLayer
{
	// Functions
public:
	virtual ~DontCrashApp();
	virtual std::unique_ptr<DC_Engine::GameStateMachine> CreateGameStateMachine() override;

};

