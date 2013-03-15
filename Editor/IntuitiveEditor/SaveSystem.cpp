#include "stdafx.h"

#include "SaveSystem.h"
#include "SaveSystemManager.h"

CSaveSystem::CSaveSystem(std::string name, std::string ext)
	: m_sName(name),
		m_sExt(ext)
{
	CSaveSystemManager::getInstance()->registerSystem(this);
}