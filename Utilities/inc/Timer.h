#pragma once

// Include from STL
#include <chrono>

namespace YS_Util
{
	/* DC_Time
		Use StartTimer function at the starting point of the application loop

		At the end of the loop use EndTimer to set prev time to calculate the time it's been taken for single loop

		Use GetDeltaTime function in middle of starting point and the end point of the loop
	*/
	class Timer
	{
	// Functions
	public:
		Timer();
		~Timer() {}

		
		void StartTimer();

		inline void EndTimer() { m_prevTime = m_currentTime; }	// Set last frame time to m_prevTime

		inline float GetDeltaTime() const { return m_deltaTime.count(); }

	// Variables
	private:
		std::chrono::duration<float> m_deltaTime;
		std::chrono::steady_clock::time_point m_currentTime;
		std::chrono::steady_clock::time_point m_prevTime;
	};
}