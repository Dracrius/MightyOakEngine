#include "CoreHeaders.h"
#include "GameCore.h"
#include "Objects/ResourceManager.h"

namespace fw {
GameCore::GameCore()
{
	m_pResourceManager = new ResourceManager();
}
GameCore::~GameCore()
{
	delete m_pResourceManager;
}
} // namespace fw
