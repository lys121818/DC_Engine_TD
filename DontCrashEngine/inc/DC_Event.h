#pragma once
#include <cstdint>

namespace DC_Engine
{
	using EventType = uint32_t;
	enum class ViewType;

	class Event
	{
	public:
		virtual ~Event() {}
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
	};
}