#pragma once
#include <DC_Process.h>

class DelayProcess : public DC_Engine::DC_Process
{
public:
	DelayProcess(float time);
	~DelayProcess();

	bool Update(float deltaTime);

private:
	float m_time;

};

