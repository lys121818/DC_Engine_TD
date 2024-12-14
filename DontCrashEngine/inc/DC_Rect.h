#pragma once
#include <Vector.h>

namespace DC_Engine
{
	struct Rect
	{
		Rect() = default;
		Rect(const float& x, const float& y, const int& width, const int& height) : 
			position(x, y),width(width),height(height){};
		Rect(const Rect& other) = default;
		Rect(Rect&& other) = default;
		Rect& operator= (const Rect& other) = default;
		Rect& operator=(Rect&& other) = default;

		bool IsContained(float x, float y);

		Vector2_f position = { 0.0f,0.0f };
		int width = 0;
		int height = 0;
		float angle = 0.0f;
	};
}