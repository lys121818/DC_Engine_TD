#include "SpawnBulletEvent.h"

// Include from DontCrashEngine
#include <DC_View.h>

SpawnBulletEvent::SpawnBulletEvent(const char* pXMLFile, const DC_Engine::Rect& ownerRect)
	:
	m_pXMLFile(pXMLFile),
	m_rect(ownerRect),
	m_viewType(DC_Engine::kBulletViewId)
{
}
