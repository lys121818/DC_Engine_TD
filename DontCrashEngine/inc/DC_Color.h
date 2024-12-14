#pragma once
#include <cstdint>

namespace DC_Engine
{
	struct Color
	{
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {};

		uint8_t r;	// Red
		uint8_t g;	// Greed
		uint8_t b;	// Blue
		uint8_t a;  // Alpha

	};
}