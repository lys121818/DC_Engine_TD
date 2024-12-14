#pragma once
//Include from STL
#include <memory>
#include <vector>

namespace DC_Engine
{
	class DC_Process;

	class ProcessManager
	{
	public:
		~ProcessManager() { AbortAllProcesses(); }

		void UpdateProcess(float deltaTime);
		inline void AttachProcess(std::shared_ptr<DC_Process> pProcess) { m_pProcesses.emplace_back(pProcess); }
		void AbortAllProcesses();

	private:
		std::vector<std::shared_ptr<DC_Process>> m_pProcesses;
	};
}