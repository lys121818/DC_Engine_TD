#include "EventManager.h"

// Include from DontCrashEngine
#include "DC_Event.h"

namespace DC_Engine
{
	size_t EventManager::AddEventListener(const EventType& type, std::function<void(Event*)> listener)
	{
		auto& listeners = m_eventListeners[type];
		for (size_t i = 0; i < listeners.size(); ++i)
		{
			if (listeners[i] == nullptr)
			{
				listeners[i] = listener;
				return i;
			}
		}

		listeners.emplace_back(listener);
		return listeners.size() - 1;
	}
	void EventManager::RemoveEventListener(const EventType& type, size_t index)
	{
		auto& listeners = m_eventListeners[type];
		if (index < listeners.size())
		{
			listeners[index] = nullptr;
		}
	}
	void EventManager::QueueEvent(std::unique_ptr<Event> pEvent)
	{
		m_queue.emplace_back(std::move(pEvent));
	}
	void EventManager::AbortEvent(const EventType& type)
	{
		// Remove all Events in queue
		auto Iter = m_queue.begin();
		while (Iter != m_queue.end())
		{
			if ((*Iter)->GetEventType() == type)
			{
				Iter = m_queue.erase(Iter);
			}
			else
				Iter++;
		}
	}
	void EventManager::ProcessEvents()
	{
		// Process all the existing queue that are added
		auto processingQueue = std::move(m_queue);
		for (auto& pEvent : processingQueue)
		{
			auto& listeners = m_eventListeners[pEvent->GetEventType()];
			for (auto& listener : listeners)
			{
				if (listener != nullptr)
					listener(pEvent.get());
			}
		}
	}
}

