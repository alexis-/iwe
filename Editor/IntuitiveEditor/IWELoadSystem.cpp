#include "stdafx.h"

#include "IWESaveSystem.h"
#include "ProjectManager.h"


// Project settings serializing
CIWEStream &operator>>(CIWEStream& inStream, CProjectManager* pProjectMgr)
{
	std::string sProjectName = "";
	std::string sProjectDir = "";

	inStream >> sProjectName;
	inStream >> sProjectDir;

	pProjectMgr->setProjectName(sProjectName);
	pProjectMgr->setProjectDir(sProjectDir);

	// TODO: Load resources here

	return inStream;
}

// Entity serializing
//CIWEStream &operator>>(CIWEStream& inStream, IEntityManager* pEntMgr)
//{
//	int iEntCount = 0;
//
//	inStream >> iEntCount;
//
//	for (int i = 0; i < iEntCount; i++)
//	{
//		IEntity*					    pEnt = pEntMgr->createEntity();
//		Ogre::Vector3					vecPos;
//		Ogre::Vector3					vecScale;
//		Ogre::Quaternion			quatOrient;
//		std::string						sModelName;
//	
//		inStream >> vecPos;
//		inStream >> quatOrient;
//		inStream >> vecScale;
//		inStream >> sModelName;
//
//		pEnt->setPosition(vecPos);
//		pEnt->setOrientation(quatOrient);
//		pEnt->setScale(vecScale);
//		pEnt->setModel(sModelName);
//	}
//
//	return inStream;
//}

// Terrain serializing
//CIWEStream &operator>>(CIWEStream& inStream, CTerrainManager* pTerrainMgr)
//{
//	int iGroupCount = 0;
//
//	inStream >> iGroupCount;
//
//	for (int i = 0; i < iGroupCount; i++)
//	{
//		char dummy = 0x00;
//		uint nID = 0;
//		float worldSize = 0;
//
//		inStream >> worldSize;
//		nID = pTerrainMgr->createTerrainGroup(worldSize);
//
//		inStream >> dummy;
//
//		while (dummy != (char)0xFF)
//		{
//			Ogre::Vector3			vecPos;
//			int								x = 0,
//												y = 0;
//
//			inStream >> vecPos;
//			inStream >> x;
//			inStream >> y;
//
//			pTerrainMgr->createTerrain(x, y, nID, vecPos, false);
//
//			inStream >> dummy;
//		}
//	}
//
//	pTerrainMgr->loadTerrains();
//	
//	return inStream;
//}

bool
Load100(CIWEStream& inStream)
{
	// Project settings
	inStream >> CProjectManager::getInstance();

	// Entities
	//inStream >> GetEntityMgr();

	// Terrain definitions
	//inStream >> CTerrainManager::getInstance(); // TODO: Move to terrain module

	return true;
}

bool
CIWESaveSystem::OnLoad(const std::string& filename)
{
	CIWEStream inStream(filename, std::ios::in | std::ios::binary);

	if (!inStream.is_open())
	{
		TRACE(("CProjectManager::saveProject() Failed to open project file " + filename + " for loading").c_str());
		return false;
	}

	int iIWEVersion = 0;

	inStream >> iIWEVersion;

	switch(iIWEVersion)
	{
	case 100:
		return Load100(inStream);
	}


	return false;
}