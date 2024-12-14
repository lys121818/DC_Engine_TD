#include "ProcessManager.h"

//Include from DontCrashEngine
#include "DC_Process.h"

namespace DC_Engine
{
	void ProcessManager::UpdateProcess(float deltaTime)
	{
		size_t processIndex = 0;

		// Update all processes
		while (processIndex != m_pProcesses.size())
		{
			auto& pProcess = m_pProcesses[processIndex];

			// Active Process
			if (pProcess->GetState() == DC_Process::State::kRunning)
			{
				pProcess->Update(deltaTime);
			}
			else if(pProcess->GetState() == DC_Process::State::kUnInitialized)
			{
				if (pProcess->Init())
				{
					pProcess->Resume();
				}
				else
				{
					m_pProcesses.erase(m_pProcesses.begin() + processIndex);
					continue;
				}
			}

			// Inactive Process
			if (pProcess->IsInactive())
			{
				DC_Process::State state = pProcess->GetState();

				if (state == DC_Process::State::kSucceeded)
				{
					pProcess->OnSuccess();
					auto child = pProcess->RemoveChild();
					if (child)
					{
						AttachProcess(child);
					}
				}
				else if (state == DC_Process::State::kFailed)
				{
					pProcess->OnFailed();
					auto child = pProcess->RemoveChild();
					if (child)
					{
						AttachProcess(child);
					}
				}
				else if (state == DC_Process::State::kAborted)
				{
					pProcess->OnAbort();
				}

				m_pProcesses.erase(m_pProcesses.begin() + processIndex);
				continue;
			}

			processIndex++;

		}//While loop
	}

	void DC_Engine::ProcessManager::AbortAllProcesses()
	{
		for (auto& pProcess : m_pProcesses)
		{
			if (pProcess->IsActive())
			{
				pProcess->Abort();
				pProcess->OnAbort();
			}
		}

		m_pProcesses.clear();
	}

}
