#include "stdafx.h"

#include "IWESaveSystem.h"
#include "ProjectManager.h"

CIWESaveSystem									g_IWESaveSystem;

CIWESaveSystem::CIWESaveSystem()
	: CSaveSystem("IWE Project File", "iwe")
{
}


// Project settings serializing
CIWEStream &operator<<(CIWEStream& outStream, CProjectManager* pProjectMgr)
{
	const std::string& sProjectName = pProjectMgr->getProjectName();
	const std::string& sProjectDir = pProjectMgr->getProjectDir();

	outStream << sProjectName;
	outStream << sProjectDir;

	return outStream;
}

// Entity serializing
//CIWEStream &operator<<(CIWEStream& outStream, IEntityManager* pEntMgr)
//{
//	outStream << pEntMgr->getCount();
//
//	for (IEntityManager::iterator it = pEntMgr->begin();
//			 it != pEntMgr->end();
//			 it++)
//	{
//		IEntity* pEnt = *it;
//
//		outStream << pEnt->getPosition();
//		outStream << pEnt->getOrientation();
//		outStream << pEnt->getScale();
//		outStream << pEnt->getModelName();
//	}
//
//	return outStream;
//}

// Terrain serializing
//CIWEStream &operator<<(CIWEStream& outStream, CTerrainManager* pTerrainMgr)
//{
//	const std::vector<Ogre::TerrainGroup*>& vecTerrainGroups = pTerrainMgr->getTerrainGroups();
//
//	outStream << vecTerrainGroups.size();
//
//	for (std::vector<Ogre::TerrainGroup*>::const_iterator it = vecTerrainGroups.begin();
//			 it != vecTerrainGroups.end();
//			 it++)
//	{
//		Ogre::TerrainGroup* pGroup = *it;
//
//		outStream << pGroup->getTerrainWorldSize();
//
//		Ogre::TerrainGroup::TerrainIterator terrainIt = pGroup->getTerrainIterator();
//		
//    while (terrainIt.hasMoreElements())
//    {
//			Ogre::TerrainGroup::TerrainSlot* pCurSlot = terrainIt.getNext();
//
//			Ogre::Terrain*		pTerrain = pCurSlot->instance;
//			int								x = pCurSlot->x,
//												y = pCurSlot->y;
//
//			if (!pTerrain)
//				continue;
//
//			outStream << (char)0x00;
//			outStream << pTerrain->getPosition();
//			outStream << x;
//			outStream << y;
//		}
//
//		outStream << (char)0xFF;
//	}
//	
//	return outStream;
//}

bool
CIWESaveSystem::OnSave(const std::string& filename)
{
	CIWEStream outStream(filename, std::ios::out | std::ios::trunc | std::ios::binary);

	if (!outStream.is_open())
	{
		TRACE(("CProjectManager::saveProject() Failed to open project file " + filename + " for saving").c_str());
		return false;
	}

	// Output version
	outStream << IWE_VERSION;

	// Project settings
	outStream << CProjectManager::getInstance();

	// Entities
	//outStream << GetEntityMgr();

	// Terrain definitions
	//outStream << CTerrainManager::getInstance(); // TODO: Move to terrain module

	return true;
}
