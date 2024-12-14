#pragma once

// Include from DontCrashEngine
#include "DC_Event.h"

// Include from STL
#include <functional>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>

namespace DC_Engine
{
	class EventManager
	{
	public:

	// Functions
		size_t AddEventListener(const EventType& type, std::function<void(Event*)> listener);
		void RemoveEventListener(const EventType& type, size_t index);

		void QueueEvent(std::unique_ptr<Event> pEvent);
		void AbortEvent(const EventType& type);

		void ProcessEvents();

	// Variables
	private:
		std::unordered_map<EventType, std::vector<std::function<void(Event*)>>> m_eventListeners;
		std::vector<std::unique_ptr<Event>> m_queue;
	};
}


