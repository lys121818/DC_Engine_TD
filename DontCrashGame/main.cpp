#include "DontCrashApp.h"
#include "ActionResult.h"

// Include from Bleach Leak Detecter
#include <BleachNew.h>
#include <tinyxml2.h>

/*
	READ ME

	[[DontCrashApp]]
	DontCrashApp is the game engine apllication created with ApplicationLayer.

	Setting for window size, name and the API type are setted inside the Init() function.
	<Window Default Settings>
	API:	SDL
	Title:	DontCrashGameEngine;
	Width:	1024;
	Height:	768;

*/


int main()
{
	
	BLEACH_INIT_LEAK_DETECTOR();
	DontCrashApp app;

	DC_Engine::ActionResult result = app.Init();

	if(result == DC_Engine::ActionResult::kSuccess)
		app.Run();

	app.Destroy();

	BLEACH_DUMP_AND_DESTROY_LEAK_DETECTOR();

	return 0;
}



/*
* #DG: second progress grade feedback
* - The engine src directory contains multiple source and header files that should really only belong to the engine view. 
*	- Sort those files that should not be included in the game view to only exist in the engine source folder
* 
* - 
*/
