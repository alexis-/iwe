#include "stdafx.h"

#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>

#include "ResourceManager.h"
#include "EngineUtils.h"

CResourceManager* CResourceManager::m_pInst = NULL;

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
  m_vecResources.clear();
}

const std::vector<std::pair<const Ogre::FileInfo, const std::string>>&
CResourceManager::getFiles()
{
  return m_vecResources;
}

void
CResourceManager::addResourceLocation(const std::string& pLocation, char* pType, char* pGroup, bool bRecursive)
{
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(pLocation, pType,
    (pGroup == "") ? Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : pGroup, bRecursive);
}

void 
CResourceManager::update()
{
  eventUpdateStarted();
  m_vecResources.clear();

  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ogre::ResourceGroupManager& pResMgr = Ogre::ResourceGroupManager::getSingleton();
	Ogre::StringVector vecGroups = pResMgr.getResourceGroups();

	for(Ogre::StringVector::iterator groupIt = vecGroups.begin();
			groupIt != vecGroups.end();
			groupIt++)
	{
		Ogre::FileInfoListPtr fileList = pResMgr.listResourceFileInfo(*groupIt);

		for (Ogre::FileInfoList::iterator fileIt = fileList->begin();
				 fileIt != fileList->end();
				 fileIt++)
    {
      eventOnFileLoaded(*fileIt, *groupIt);
      m_vecResources.push_back(std::pair<const Ogre::FileInfo, const std::string>(*fileIt, *groupIt));
    }
	}

  eventUpdateFinished();
}


void
CResourceManager::ReloadMaterial(const std::string& materialName, const std::string& groupName,
                                 const std::string& filename, bool parseMaterialScript)
{
  CEngineUtils::ReloadMaterial(materialName, groupName, filename, parseMaterialScript);
}

void
CResourceManager::updateOnEveryRenderable()
{
  CEngineUtils::updateOnEveryRenderable();
}

void*
CResourceManager::getBitmapMaterial(const std::string& matName, const std::string& groupName,
                                    uint& width, uint& height, uchar& bpp)
{
  return CEngineUtils::getBitmapMaterial(matName, groupName, width, height, bpp);
}

CResourceManager*
CResourceManager::getInstance()
{
	if (!m_pInst)
		m_pInst = NEW CResourceManager();

	return m_pInst;
}

CResourceManager*
getResourceMgr()
{
	return CResourceManager::getInstance();
}