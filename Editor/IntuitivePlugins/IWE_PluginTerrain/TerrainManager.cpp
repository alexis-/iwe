#include "IWEPlugin.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreMaterialManager.h>

#include "TerrainManager.h"
#include "TerrainMaterialGeneratorB.h"
//#include "EngineUtils.h"

CTerrainManager*									CTerrainManager::m_pInst = NULL;
int                               CTerrainManager::m_iTerrainSize = 0;
float                             CTerrainManager::m_fTerrainWorldSize = 0.0f;


namespace Ogre
{
	void TerrainFix::finishDerivedWork() { waitForDerivedProcesses(); }
}

CTerrainManager*
CTerrainManager::getInstance()
{
	if (!m_pInst)
		m_pInst = new CTerrainManager();

	return m_pInst;
}

CTerrainManager::CTerrainManager()
	: m_pLastTerrain(NULL)
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();
  m_pDecalTexture = Ogre::TextureManager::getSingletonPtr()->createManual("IWETerrainDecalTexture",
																																					"Editor",
																																					Ogre::TEX_TYPE_2D, 256, 256, 1, 1,
																																					Ogre::PF_A8R8G8B8,
																																					Ogre::TU_DYNAMIC_WRITE_ONLY);

	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();

	Ogre::Light* l = pSceneMgr->createLight("terrainLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(lightdir);
	l->setDiffuseColour(Ogre::ColourValue::White);
	l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	
	m_pTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	m_iTerrainSize = 513;
	m_fTerrainWorldSize = 256.0f;

	m_pTerrainGlobals->setMaxPixelError(1);
  m_pTerrainGlobals->setUseRayBoxDistanceCalculation(true);
  
  m_pTerrainGlobals->setCompositeMapSize(1024);
  m_pTerrainGlobals->setCompositeMapDistance(2000);
  m_pTerrainGlobals->setLightMapSize(1024);
  m_pTerrainGlobals->setLayerBlendMapSize(1024);
  m_pTerrainGlobals->setUseVertexCompressionWhenAvailable(false);
  m_pTerrainGlobals->setSkirtSize(5);

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	m_pTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
	m_pTerrainGlobals->setCompositeMapAmbient(pSceneMgr->getAmbientLight());
	m_pTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());

	//pTerrainGlobalOptions->setMaxPixelError(1);
 // pTerrainGlobalOptions->setCompositeMapSize(1024);
 // pTerrainGlobalOptions->setCompositeMapDistance(2000);
 // pTerrainGlobalOptions->setLightMapSize(1024);
 // pTerrainGlobalOptions->setLayerBlendMapSize(1024);
 // pTerrainGlobalOptions->setSkirtSize(10);
 // pTerrainGlobalOptions->setUseRayBoxDistanceCalculation(false);
	//pTerrainGlobalOptions->setCastsDynamicShadows(false);
	
	
	m_pDecalFrustum = OGRE_NEW Ogre::Frustum();
  m_pDecalNode = pSceneMgr->getRootSceneNode()->createChildSceneNode("IWETerrainDecalNode");
  m_pDecalNode->setPosition(99999,-99999,99999);
  m_pDecalNode->attachObject(m_pDecalFrustum);
  m_pDecalFrustum->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
  m_pDecalNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
  m_pDecalFrustum->setFOVy(Ogre::Degree(45));
  m_pDecalFrustum->setNearClipDistance(10);
  m_pDecalFrustum->setOrthoWindow(10, 10);
  m_pDecalFrustum->setVisible(false);
	m_pDecalFrustum->setVisibilityFlags(1);
  //m_pDecalFrustum->setRenderQueueGroup(0x70);


	Ogre::MaterialManager&			lMaterialManager = Ogre::MaterialManager::getSingleton();
	Ogre::MaterialPtr						lCreatedMaterial = lMaterialManager.getByName("Editor/AlphaBrushes/Brush");
	Ogre::Pass*									lPass = lCreatedMaterial->getTechnique(0)->getPass(0);
	Ogre::TextureUnitState*			lTexState = lPass->getTextureUnitState(0);
	lTexState->setTextureName("IWETerrainDecalTexture");

  lCreatedMaterial->load();

	Ogre::TerrainMaterialGeneratorPtr matGenPtr(OGRE_NEW Ogre::TerrainMaterialGeneratorB(m_pDecalFrustum));
	m_pTerrainGlobals->setDefaultMaterialGenerator(matGenPtr);
}

CTerrainManager::~CTerrainManager()
{
	clear();

	m_pDecalNode->detachAllObjects();
	GetRenderEngine()->getSceneManager()->destroySceneNode(m_pDecalNode);
	delete m_pDecalFrustum;
}

void
CTerrainManager::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap*		blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap*		blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real										minHeight0 = 70;
	Ogre::Real										fadeDist0 = 40;
	Ogre::Real										minHeight1 = 70;
	Ogre::Real										fadeDist1 = 15;

	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();

	blendMap0->update();
	blendMap1->update();
}

bool
CTerrainManager::terrainIsDefined(long x, long y)
{
	if (m_mapTerrains.find(std::pair<int, int>(x, y)) != m_mapTerrains.end())
		return true;

	return false;
}

UINT
CTerrainManager::createTerrainGroup(float worldSize)
{
	worldSize = (int)worldSize;

	if ((int)worldSize % 2 != 0)
		worldSize = ((int)worldSize) + 1;

  int iTerrSize = Ogre::Math::Pow(2, Ogre::Math::Floor(Ogre::Math::Log2(worldSize)) + 2) + 1;

	Ogre::TerrainGroup* pTerrGroup = OGRE_NEW Ogre::TerrainGroup(GetRenderEngine()->getSceneManager(),
			Ogre::Terrain::ALIGN_X_Z, iTerrSize, worldSize);

	if (!pTerrGroup)
	{
		printf("Error while creating new terrain group");
		return -1;
	}
	
	m_vecTerrainGroups.push_back(pTerrGroup);
	int nID = m_vecTerrainGroups.size() - 1;
		
	pTerrGroup->setFilenameConvention("terrain_" + Ogre::StringConverter::toString(nID), "dat"); // TODO: Change this depending on selected World

	Ogre::Terrain::ImportData& defaultimp = pTerrGroup->getDefaultImportSettings();
	defaultimp.terrainSize = m_iTerrainSize;
	defaultimp.worldSize = worldSize;
	defaultimp.inputScale = 600;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;

	// textures
	defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 15;
	defaultimp.layerList[0].textureNames.push_back("grass_green-01_diffusespecular.png");
	defaultimp.layerList[0].textureNames.push_back("grass_green-01_normalheight.png");
	defaultimp.layerList[1].worldSize = 15;
	defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_diffusespecular.png");
	defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_normalheight.png");
	defaultimp.layerList[2].worldSize = 15;
	defaultimp.layerList[2].textureNames.push_back("dirt_grayrocky_diffusespecular.png");
	defaultimp.layerList[2].textureNames.push_back("dirt_grayrocky_normalheight.png");

	return nID;
}

bool
CTerrainManager::checkTerrainIntersect(Ogre::RealRect rect)
{
	for (std::vector<Ogre::RealRect>::iterator it = m_vecTerrainBounds.begin();
			 it != m_vecTerrainBounds.end();
			 it++)
		if (!(*it).intersect(rect).isNull())
			return true;

	return false;
}

bool
CTerrainManager::isValidGroupID(UINT nGroupID)
{
	return (nGroupID < m_vecTerrainGroups.size());
}

bool
CTerrainManager::isValidTerrainSpot(int iX, int iY, const Ogre::RealRect& rect)
{
	return (!terrainIsDefined(iX, -iY) && !checkTerrainIntersect(rect));
}

bool
CTerrainManager::createTerrain(long iX, long iY, UINT nGroupID, const Ogre::Vector3& terrainPosition)
{
	if (!isValidGroupID(nGroupID))
		return false;
	
	Ogre::TerrainGroup*			pTerrainGroup = m_vecTerrainGroups[nGroupID];
	float										fHalfTerrainSize = pTerrainGroup->getTerrainWorldSize() / 2;
	Ogre::RealRect					rect(terrainPosition.x - fHalfTerrainSize, terrainPosition.z - fHalfTerrainSize,
															 terrainPosition.x + fHalfTerrainSize, terrainPosition.z + fHalfTerrainSize);

	pTerrainGroup->defineTerrain(iX, -iY, 0.0f);
	pTerrainGroup->loadTerrain(iX, -iY, true);

	//s_terrainCreationInfo sTerrainInfos;
	//sTerrainInfos.iX = iX;
	//sTerrainInfos.iY = iY;
	//sTerrainInfos.vecPos = terrainPosition;
	//sTerrainInfos.pTerrainGroup = pTerrainGroup;
	//
	//m_vecTerrainBounds.push_back(rect);
	//m_vecPendingTerrains.push_back(sTerrainInfos);
	Ogre::Terrain* pTerrain = pTerrainGroup->getTerrain(iX, -iY);

	if (pTerrain)
	{
		initBlendMaps(pTerrain);

		Ogre::Vector3 vecFinalTerrainPosition = terrainPosition;
		vecFinalTerrainPosition.x = (int)vecFinalTerrainPosition.x;
		vecFinalTerrainPosition.y = (int)vecFinalTerrainPosition.y;
		vecFinalTerrainPosition.z = (int)vecFinalTerrainPosition.z;

		pTerrain->setPosition(vecFinalTerrainPosition);

		pTerrain->setVisibilityFlags(1);
		
#if 0
		{
			pTerrain->setGlobalColourMapEnabled(true, 1024);
			Ogre::TexturePtr colorMapTex = pTerrain->getGlobalColourMap();

			Ogre::HardwarePixelBufferSharedPtr pixBuffPtr = colorMapTex->getBuffer();
			char* dataPtr = (char*)pixBuffPtr->lock(Ogre::HardwareBuffer::HBL_NORMAL);

			int max = 4 * pixBuffPtr->getWidth() * pixBuffPtr->getHeight();
			
			for (int i = 0; i < max; i += 4)
				dataPtr[i] = dataPtr[i + 1] = dataPtr[i + 2] = 255;

			pixBuffPtr->unlock();
		}
#endif

		m_mapTerrains[std::pair<int, int>((int)iX, (int)-iY)] = pTerrain;

		m_vecTerrainBounds.push_back(rect);
		m_vecTerrains.push_back(pTerrain);

		pTerrain->freeTemporaryResources();

		Ogre::TerrainFix* pFixTerrain = static_cast<Ogre::TerrainFix*>(pTerrain);
		pFixTerrain->finishDerivedWork();
	}
	else
	{
		printf("Error while creating terrain !\n"); // TODO: Log this
		return false;
	}

	pTerrainGroup->freeTemporaryResources();

	return true;
}

bool
CTerrainManager::createTerrain(long iX, long iY, UINT nGroupID)
{
	if (!isValidGroupID(nGroupID))
		return false;
	
	Ogre::TerrainGroup*			pTerrainGroup = m_vecTerrainGroups[nGroupID];
	float										fTerrainSize = pTerrainGroup->getTerrainWorldSize();
  Ogre::RealRect					rect(iX * fTerrainSize,                 iY * fTerrainSize,
															 iX * fTerrainSize + fTerrainSize,  iY * fTerrainSize + fTerrainSize);

	pTerrainGroup->defineTerrain(iX, -iY, 0.0f);
	pTerrainGroup->loadTerrain(iX, -iY, true);

	//s_terrainCreationInfo sTerrainInfos;
	//sTerrainInfos.iX = iX;
	//sTerrainInfos.iY = iY;
	//sTerrainInfos.vecPos = terrainPosition;
	//sTerrainInfos.pTerrainGroup = pTerrainGroup;
	//
	//m_vecTerrainBounds.push_back(rect);
	//m_vecPendingTerrains.push_back(sTerrainInfos);
	Ogre::Terrain* pTerrain = pTerrainGroup->getTerrain(iX, -iY);

	if (pTerrain)
	{
		initBlendMaps(pTerrain);

		pTerrain->setVisibilityFlags(1);
		
#if 0
		{
			pTerrain->setGlobalColourMapEnabled(true, 1024);
			Ogre::TexturePtr colorMapTex = pTerrain->getGlobalColourMap();

			Ogre::HardwarePixelBufferSharedPtr pixBuffPtr = colorMapTex->getBuffer();
			char* dataPtr = (char*)pixBuffPtr->lock(Ogre::HardwareBuffer::HBL_NORMAL);

			int max = 4 * pixBuffPtr->getWidth() * pixBuffPtr->getHeight();
			
			for (int i = 0; i < max; i += 4)
				dataPtr[i] = dataPtr[i + 1] = dataPtr[i + 2] = 255;

			pixBuffPtr->unlock();
		}
#endif

		m_mapTerrains[std::pair<int, int>((int)iX, (int)-iY)] = pTerrain;

		m_vecTerrainBounds.push_back(rect);
		m_vecTerrains.push_back(pTerrain);

		pTerrain->freeTemporaryResources();

		Ogre::TerrainFix* pFixTerrain = static_cast<Ogre::TerrainFix*>(pTerrain);
		pFixTerrain->finishDerivedWork();
	}
	else
	{
		printf("Error while creating terrain !\n"); // TODO: Log this
		return false;
	}

	pTerrainGroup->freeTemporaryResources();

	return true;
}

float
CTerrainManager::getGroupSize(UINT nGroupID)
{
	if (!isValidGroupID(nGroupID))
		return 0;

	return m_vecTerrainGroups[nGroupID]->getTerrainWorldSize();
}

void
CTerrainManager::clear()
{
	while (m_vecTerrainGroups.size() > 0)
	{
		Ogre::TerrainGroup* pGroup = m_vecTerrainGroups.back();

		pGroup->removeAllTerrains();
		OGRE_DELETE pGroup;

		m_vecTerrainGroups.pop_back();
	}

	m_mapTerrains.clear();
	m_vecTerrainGroups.clear();
	m_vecTerrainBounds.clear();
	m_vecTerrains.clear();
}

float
CTerrainManager::getTerrainHeight(float x, float z)
{
	if (x > m_lastTerrainBounds.left && x < m_lastTerrainBounds.right
			&& z > m_lastTerrainBounds.bottom && z < m_lastTerrainBounds.top)
		return m_pLastTerrain->getHeightAtWorldPosition(Ogre::Vector3(x, 0.0f, z));

	for (int i = 0; i < m_vecTerrainBounds.size(); i++)
	{
		Ogre::RealRect rect = m_vecTerrainBounds[i];

		if (x > rect.left && x < rect.right
				&& z > rect.bottom && z < rect.top)
		{
			Ogre::Terrain* pTerrain = m_vecTerrains[i];

			m_pLastTerrain = pTerrain;
			m_lastTerrainBounds = rect;

			return pTerrain->getHeightAtWorldPosition(Ogre::Vector3(x, 0.0f, z));
		}
	}

	return 0.0f;
}

bool
CTerrainManager::getTerrainCoord(float x, float y, IViewport* pViewport, Ogre::Vector3& vecOut)
{
	Ogre::Ray				ray = pViewport->getViewportRay(x, y);

	return getTerrainCoord(ray, pViewport->getCamera()->getPosition(), vecOut);
}

bool
CTerrainManager::getTerrainCoord(const Ogre::Ray& ray, const Ogre::Vector3& vecCamPos, Ogre::Vector3& vecOut)
{
	float						dist = -1.0f;
	bool						ret = false;

	for (std::vector<Ogre::TerrainGroup*>::iterator it = m_vecTerrainGroups.begin();
			 it != m_vecTerrainGroups.end();
			 it++)
	{
		Ogre::TerrainGroup* pGroup = *it;
		Ogre::TerrainGroup::TerrainIterator terrainIt = pGroup->getTerrainIterator();
		
    while (terrainIt.hasMoreElements())
    {
			Ogre::TerrainGroup::TerrainSlot* pCurSlot = terrainIt.getNext();

			Ogre::Terrain*		pTerrain = pCurSlot->instance;
			
			std::pair<bool, Ogre::Vector3> res = pTerrain->rayIntersects(ray);
			
			if (res.first)
			{
				Ogre::Vector3 vecPos = res.second;
				float newDist = vecPos.distance(vecCamPos);

				if (dist == -1.0f || newDist < dist)
				{
					dist = newDist;
					vecOut = vecPos;
					ret = true;
				}
			}
		}
	}

	return ret;
}

bool
CTerrainManager::getTerrainDatas(float x, float y, IViewport* pViewport, Ogre::Vector3& vecOut, Ogre::Terrain*& pTerrainOut)
{
	Ogre::Vector3		vecCamPos = pViewport->getCamera()->getPosition();
	Ogre::Ray				ray = pViewport->getViewportRay(x, y);
	float						dist = -1.0f;
	bool						ret = false;

	for (std::vector<Ogre::TerrainGroup*>::iterator it = m_vecTerrainGroups.begin();
			 it != m_vecTerrainGroups.end();
			 it++)
	{
		Ogre::TerrainGroup* pGroup = *it;
		Ogre::TerrainGroup::TerrainIterator terrainIt = pGroup->getTerrainIterator();
		
    while (terrainIt.hasMoreElements())
    {
			Ogre::TerrainGroup::TerrainSlot* pCurSlot = terrainIt.getNext();

			Ogre::Terrain*		pTerrain = pCurSlot->instance;
			
			std::pair<bool, Ogre::Vector3> res = pTerrain->rayIntersects(ray);
			
			if (res.first)
			{
				Ogre::Vector3 vecPos = res.second;
				float newDist = vecPos.distance(vecCamPos);

				if (dist == -1.0f || newDist < dist)
				{
					dist = newDist;
					vecOut = vecPos;
					pTerrainOut = pTerrain;

					ret = true;
				}
			}
		}
	}

	return ret;
}

void
CTerrainManager::setDecalVisible(bool bShow)
{
	m_pDecalFrustum->setVisible(bShow);
}

void
CTerrainManager::setDecalPosition(Ogre::Vector2 vecPos)
{
	m_pDecalNode->setPosition(Ogre::Vector3(vecPos.x,
																					getTerrainHeight(vecPos.x, vecPos.y),
																					vecPos.y));
}

void
CTerrainManager::setDecalPosition(Ogre::Vector3 vecPos)
{
	m_pDecalNode->setPosition(vecPos);
}

#include <OgreHardwarePixelBuffer.h>

void
CTerrainManager::setDecalTexture(std::string sTexName)
{
	m_decalImg.load(sTexName, "EditorAlphaBrushes");

	//m_pDecalTexture->loadImage(img);

	{
    int iTotalSize = m_decalImg.getWidth() * m_decalImg.getHeight() * 4;
    unsigned char *dataptr = OGRE_ALLOC_T(unsigned char, iTotalSize, Ogre::MEMCATEGORY_GEOMETRY);
    Ogre::PixelBox resultbox(m_decalImg.getWidth(), m_decalImg.getHeight(), 1, Ogre::PF_R8G8B8, dataptr);
    Ogre::PixelUtil::bulkPixelConversion(m_decalImg.getPixelBox(), resultbox);

    resultbox.setConsecutive();
    m_pDecalTexture->setHeight(m_decalImg.getHeight());
    m_pDecalTexture->setWidth(m_decalImg.getWidth());
    Ogre::HardwarePixelBufferSharedPtr ptr = m_pDecalTexture->getBuffer();
    ptr->blitFromMemory(resultbox);
    OGRE_FREE(dataptr, Ogre::MEMCATEGORY_GEOMETRY);
	}
	
	// TODO: Ugly hack, fix this !!!!
	//{
	//	Ogre::MaterialManager&			lMaterialManager = Ogre::MaterialManager::getSingleton();
	//	Ogre::MaterialPtr						lCreatedMaterial = lMaterialManager.getByName("Editor/AlphaBrushes/Brush");
	//	Ogre::Pass*									lPass = lCreatedMaterial->getTechnique(0)->getPass(0);
	//	Ogre::TextureUnitState*			lTexState = lPass->getTextureUnitState(0);
	//	lTexState->setTextureName("IWETerrainDecalTexture");

 //   lCreatedMaterial->load();
	//	//CMeshDecal* pDecal = new CMeshDecal("Editor/AlphaBrushes/Brush");
	//	//delete pDecal;
	//}

	m_pDecalTexture->convertToImage(m_decalImg);
}

void
CTerrainManager::setDecalSize(float size)
{
  m_pDecalFrustum->setNearClipDistance(size);
  m_pDecalFrustum->setOrthoWindow(size, size);
}