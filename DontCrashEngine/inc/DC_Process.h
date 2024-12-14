#pragma once

//Include from STL
#include <functional>
#include <memory>

namespace DC_Engine
{
	class DC_Process
	{
	public:
		enum class State
		{
			kUnInitialized = 0,
			kRunning,
			kPaused,
			kSucceeded,
			kFailed,
			kAborted
		};

	// Functions
	public:
		DC_Process() : m_currentState(State::kUnInitialized) {}
		virtual ~DC_Process() {};

		virtual bool Init() { return true; }
		virtual bool Update(float) = 0;

		// Sets the current state
		inline void Succeed() { m_currentState = State::kSucceeded; }
		inline void Fail() { m_currentState = State::kFailed; }
		inline void Abort() { m_currentState = State::kAborted; }
		inline void Resume() { m_currentState = State::kRunning; }
		inline void Paused() { m_currentState = State::kPaused; }

		// Checking states
		inline bool IsActive() const { return m_currentState == State::kRunning || m_currentState == State::kPaused; }
		inline bool IsInactive() const { return m_currentState == State::kSucceeded || m_currentState == State::kFailed || m_currentState == State::kAborted; }

		// Setting child for after process
		inline void AttachChild(std::shared_ptr<DC_Process> child) { m_pChild = child; }
		std::shared_ptr<DC_Process> RemoveChild();

		// Sets the callback
		inline void SetSuccessCallback(std::function<void()> callback) { m_successCallback = callback; }
		inline void SetFailedCallback(std::function<void()> callback) { m_failedCallback = callback; }
		inline void SetAbortCallback(std::function<void()> callback) { m_abortCallback = callback; }

		// Return true if the function runs correctly
		bool OnSuccess();
		bool OnFailed();
		bool OnAbort();

		// Returns the current state
		inline State GetState() const { return m_currentState; }
	private:


	// Variable
	private:
		State m_currentState;
		std::shared_ptr<DC_Process> m_pChild;
		// callbacks function
		std::function<void()> m_successCallback;
		std::function<void()> m_failedCallback;
		std::function<void()> m_abortCallback;
	};
}