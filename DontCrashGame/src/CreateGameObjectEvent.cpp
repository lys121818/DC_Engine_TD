#include "CreateGameObjectEvent.h"

// Include from DontCrashEngine

CreateGameObjectEvent::CreateGameObjectEvent(const char* pXMLFile, DC_Engine::DC_View::Id viewType)
	:
	m_pXMLFile(pXMLFile),
	m_viewType(viewType)
{
}
