
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreMovableObject.h>
#include <OgreSubEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreManualObject.h>
#include <Terrain\OgreTerrainGroup.h>
#include <Terrain\OgreTerrain.h>

#include "iwebase.h"
#include "TestMesh.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

CMeshDecal::CMeshDecal(const std::string& materialName, Ogre::SceneManager* pSceneMgr)
  : m_pSceneMgr(pSceneMgr)
{
	m_pManualObj = pSceneMgr->createManualObject();
	m_pManualObj->setVisibilityFlags(1);

	m_pNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
	m_pNode->attachObject(m_pManualObj);

	int x_size = 2;  // number of polygons
	int z_size = 2;

	m_pManualObj->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	for (int i=0; i<=x_size; i++)
	{
		for (int j=0; j<=z_size; j++)
		{
			m_pManualObj->position(Ogre::Vector3((Ogre::Real)i, 0, (Ogre::Real)j));
			m_pManualObj->normal(0.0f, 1.0f, 0.0f);
			m_pManualObj->textureCoord((float)i / (float)x_size, (float)j / (float)z_size, 1.0f);
			m_pManualObj->textureCoord(0.0f, 0.0f, 0.0f);
			m_pManualObj->textureCoord(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	for (int i=0; i<x_size; i++)
	{
		for (int j=0; j<z_size; j++)
		{
			m_pManualObj->quad(i * (x_size + 1) + j,
												 i * (x_size + 1) + j + 1,
												 (i + 1) * (x_size + 1) + j + 1,
												 (i + 1) * (x_size + 1) + j);
		}																			 
	}

	m_pManualObj->end();
}

CMeshDecal::~CMeshDecal()
{
	//GetRenderEngine()->getSceneManager()->getRootSceneNode()->detachObject(m_pManualObj);
	m_pSceneMgr->destroySceneNode(m_pNode);
	m_pSceneMgr->destroyManualObject(m_pManualObj);
}

void
CMeshDecal::setVisible(bool bVisible)
{
	m_pNode->setVisible(bVisible);
}

void
CMeshDecal::update(float x, float z, float rad, const std::string& materialName)
{
	m_pManualObj->setMaterialName(0, materialName);

	update(x, z, rad);
}

void
CMeshDecal::update(float x, float z, float rad)
{
	//Ogre::Real x1 = x - rad;
	//Ogre::Real z1 = z - rad;

	//int x_size = 4;  // number of polygons
	//int z_size = 4;

	//Ogre::Real x_step = rad * 2/ x_size;
	//Ogre::Real z_step = rad * 2/ z_size;

	//m_pManualObj->beginUpdate(0);
	//// redefine vertices
	//for (int i=0; i<=x_size; i++)
	//{
	//	for (int j=0; j<=z_size; j++)
	//	{
	//		m_pManualObj->position(Ogre::Vector3(x1, CTerrainManager::getInstance()->getTerrainHeight(x1, z1) + 1, z1));
	//		m_pManualObj->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
	//		z1 += z_step;
	//	}
	//	x1 += x_step;
	//	z1 = z - rad;
	//}
	//// redefine quads
	//for (int i=0; i<x_size; i++)
	//{
	//	for (int j=0; j<z_size; j++)
	//	{
	//		m_pManualObj->quad(i * (x_size + 1) + j,
	//											 i * (x_size + 1) + j + 1,
	//											 (i + 1) * (x_size + 1) + j + 1,
	//											 (i + 1) * (x_size + 1) + j);
	//	}
	//}

	struct s_vertexDatas
	{
		s_vertexDatas(Ogre::Vector3 vecPos, Ogre::Vector2 vecTexPos, float fHeight)
			: m_bIgnore(false)
		{
			m_vecPos = vecPos;
			m_vecTexPos = vecTexPos;
			m_fHeight = fHeight;
		}

		Ogre::Vector3				m_vecPos;
		Ogre::Vector2				m_vecTexPos;
		float								m_fHeight;
		bool								m_bIgnore;
	};

	Ogre::Real	fXStart = x - rad;
	Ogre::Real	fZStart = z - rad;
	Ogre::Real	fXEnd = x + rad;
	Ogre::Real	fZEnd = z + rad;
	
	int					iStartX = Ogre::Math::Floor(fXStart);
	int					iStartZ = Ogre::Math::Floor(fZStart);
	int					iEndX = Ogre::Math::Ceil(fXEnd);
	int					iEndZ = Ogre::Math::Ceil(fZEnd);

	int					iXSize = iEndX - iStartX + 1;
	int					iZSize = iEndZ - iStartZ + 1;
	int					iSize = iXSize * iZSize;

	float				fXLength = fXEnd - fXStart;
	float				fZLength = fZEnd - fZStart;

	s_vertexDatas** pVerticesDatas = new s_vertexDatas*[iSize];

	int					it = 0;

	// Initialize datas for optimization and vertice creation
	for (int j = iStartZ; j <= iEndZ; j++)
	{
		for (int i = iStartX; i <= iEndX; i++)
		{
			Ogre::Vector3			vecPos;
			Ogre::Vector2			vecTexPos;
			float							fHeight = 0;

			{
				float fX = 0;
				float fZ = 0;

				if (i == iStartX || i == iEndX)
				{
					if (j == iStartZ || j == iEndZ)
						fZ = (j == iStartZ) ? fZStart : fZEnd;
					else
						fZ = j;

					fX = (i == iStartX) ? fXStart : fXEnd;
				}
				else if (j == iStartZ || j == iEndZ)
				{
					fX = i;
					fZ = (j == iStartZ) ? fZStart : fZEnd;
				}
				else
				{
					fX = i;
					fZ = j;
				}
				
				fHeight = 0.0f; //CTerrainManager::getInstance()->getTerrainHeight(fX, fZ) + 1;
				vecPos = Ogre::Vector3(fX, fHeight, fZ);
				vecTexPos = Ogre::Vector2((fX - fXStart) / fXLength, (fZ - fZStart) / fZLength);
			}

			pVerticesDatas[it] = new s_vertexDatas(vecPos, vecTexPos, fHeight);

			it++;
		}
	}

	m_pManualObj->beginUpdate(0);

	for (it = 0; it < iSize; it++)
	{
			m_pManualObj->position(pVerticesDatas[it]->m_vecPos);
			m_pManualObj->textureCoord(pVerticesDatas[it]->m_vecTexPos);
	}
	
	for (int j = 0; j < iZSize - 1; j++)
	{
		for (int i = 0; i < iXSize - 1; i++)
		{
			m_pManualObj->quad((j + 1) * iXSize + i,
												 (j + 1) * iXSize + i + 1,
												 j * iXSize + i + 1,
												 j * iXSize + i);
		}
	}

	m_pManualObj->end();

	for (int i = 0; i < iSize; i++)
		delete pVerticesDatas[i];

	delete [] pVerticesDatas;
}

float
averageHeight(float** heightField, int x, int y, int max)
{
	float avg = 0;
	int count = 0;

	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 0 && i < max && j >= 0 && j < max)
			{
				if (heightField[i][j] != -1)
				{
					avg += heightField[i][j];
					count++;
				}
			}
		}
	}

	if (count == 0)
		return 0;

	return avg / count;
}

void
CTestMesh::update(float x, float z, float rad)
{
	int							terrainSize = 128;
	float**					terrainHeight;

	terrainHeight = new float*[terrainSize];

	for (int i = 0; i < terrainSize; i++)
		terrainHeight[i] = new float[terrainSize];

	srand((unsigned int)time(0));
	
	for (int j = 0; j < terrainSize; j++)
		for (int i = 0; i < terrainSize; i++)
			terrainHeight[i][j] = -1;

	
	for (int j = 0; j < terrainSize; j++)
		for (int i = 0; i < terrainSize; i++)
			terrainHeight[i][j] = averageHeight((float**)terrainHeight, i, j, terrainSize) + ((float)(std::rand() % 100) / 100.0f) - 0.5f;

	
	m_pManualObj->beginUpdate(0);

	int iTileCount = 0;
	int iTile = 0;
	
	for (int j = 0; j < terrainSize - 1; j++)
	{
		for (int i = 0; i < terrainSize - 1; i++)
		{
			Ogre::Vector3 A1(i, terrainHeight[i][j], j);
			Ogre::Vector3 B1(i, terrainHeight[i][j + 1], j + 1);
			Ogre::Vector3 B2(i + 1, terrainHeight[i + 1][j + 1], j + 1);
			Ogre::Vector3 A2(i + 1, terrainHeight[i + 1][j], j);
			
			m_pManualObj->position(A2);
			m_pManualObj->normal(- (B1 - A2).crossProduct(A1 - A2).normalisedCopy());
			m_pManualObj->textureCoord((float)(i + 1) / 10, terrainHeight[i + 1][j] / 10, (float)j / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
			m_pManualObj->position(A1);
			m_pManualObj->normal(- (B1 - A1).crossProduct(A2 - A1).normalisedCopy());
			m_pManualObj->textureCoord((float)i / 10, terrainHeight[i][j] / 10, (float)j / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
			m_pManualObj->position(B1);
			m_pManualObj->normal(- (A1 - B1).crossProduct(A2 - B1).normalisedCopy());
			m_pManualObj->textureCoord((float)i / 10, terrainHeight[i][j + 1] / 10, (float)(j + 1) / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
			
			m_pManualObj->position(B2);
			m_pManualObj->normal(- (B1 - B2).crossProduct(A2 - B2).normalisedCopy());
			m_pManualObj->textureCoord((float)(i + 1) / 10, terrainHeight[i + 1][j + 1] / 10, (float)(j + 1) / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
			m_pManualObj->position(A2);
			m_pManualObj->normal(- (B1 - A2).crossProduct(B2 - A2).normalisedCopy());
			m_pManualObj->textureCoord((float)(i + 1) / 10, terrainHeight[i + 1][j] / 10, (float)j / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
			m_pManualObj->position(B1);
			m_pManualObj->normal(- (B2 - B1).crossProduct(A2 - B1).normalisedCopy());
			m_pManualObj->textureCoord((float)i / 10, terrainHeight[i][j + 1] / 10, (float)(j + 1) / 10);
			m_pManualObj->textureCoord((float)iTile, 0.0f, (iTile != 4 ? 1.0f : 0.0f));
			m_pManualObj->textureCoord(0.25f, 1.0f, 512.0f, 9.0f);
		}

		if (iTileCount >= 9)
		{
			iTileCount = 0;

			if (iTile < 4)
				iTile++;
			else
				iTile = 0;
		}
		else
			iTileCount++;
	}
	
	//for (int j = 0; j < terrainSize - 1; j++)
	//{
	//	for (int i = 0; i < terrainSize - 1; i++)
	//	{
	//		m_pManualObj->quad((j + 1) * terrainSize + i,
	//											 (j + 1) * terrainSize + i + 1,
	//											 j * terrainSize + i + 1,
	//											 j * terrainSize + i);
	//	}
	//}

	m_pManualObj->end();
	//m_pNode->setPosition(Ogre::Vector3(x, 0, z));
	m_pNode->setScale(Ogre::Vector3(5.0));

	for (int i = 0; i < terrainSize; i++)
		delete [] terrainHeight[i];

	delete [] terrainHeight;
}