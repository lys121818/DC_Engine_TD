#include "DelayProcess.h"

DelayProcess::DelayProcess(float time)
	:m_time(time)
{
}

DelayProcess::~DelayProcess()
{
}

bool DelayProcess::Update(float deltaTime)
{
	if (m_time <= deltaTime)
		Succeed();
	else
		m_time -= deltaTime;

	return true;
}
