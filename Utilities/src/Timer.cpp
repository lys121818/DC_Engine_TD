#include "Timer.h"


namespace YS_Util
{
	Timer::Timer():
		m_currentTime(std::chrono::steady_clock::now()),
		m_deltaTime(std::chrono::duration<float>::min()),
		m_prevTime(std::chrono::steady_clock::now())
	{
	}
	void Timer::StartTimer()
	{
		// Set current time
		m_currentTime = std::chrono::high_resolution_clock::now();

		// set delta time by difference between previouse time and current time
		m_deltaTime = m_currentTime - m_prevTime;
	}
}