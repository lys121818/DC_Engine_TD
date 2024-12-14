#include "DC_Rect.h"

namespace DC_Engine
{
	bool DC_Engine::Rect::IsContained(float x, float y)
	{
		if (x >= position.x && x <= (position.x + width) &&
			y >= position.y && y <= (position.y + height))
		{
			return true;
		}
		return false;
	}

}


