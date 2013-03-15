#include "stdafx.h"

#include "SaveSystemManager.h"

CSaveSystemManager*							CSaveSystemManager::m_pInst = NULL;

CSaveSystemManager*
CSaveSystemManager::getInstance()
{
	if (!m_pInst)
		m_pInst = NEW CSaveSystemManager();

	return m_pInst;
}

CSaveSystemManager::CSaveSystemManager()
{
}

void
CSaveSystemManager::registerSystem(CSaveSystem* pSystem)
{
	if (!pSystem)
		return;

	m_mapSaveSystems[pSystem->getExt()] = pSystem;
}

CSaveSystem*
CSaveSystemManager::getFromExt(const std::string& sExt)
{
	std::map<std::string, CSaveSystem*>::iterator it = m_mapSaveSystems.find(sExt);

	if (it != m_mapSaveSystems.end())
		return (*it).second;

	return NULL;
}
