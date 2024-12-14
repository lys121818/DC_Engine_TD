#pragma once
#include <string>


/*========================================================================

  Type - Returns the type of the api with enum state

  ActionResult - Returns the result of state with enum state

  WindowConfig - It contains the information of the game window configuration

========================================================================*/

namespace DC_Engine
{
	enum class ActionResult
	{
		kSuccess = 0,
		kWindowInitFail = 1,
		kWindowCreateFail,
		kAPIInitFail,
	};
}