#include "stdafx.h"

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreMovableObject.h>
#include <OgreSubEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreManualObject.h>
#include <Terrain\OgreTerrainGroup.h>
#include <Terrain\OgreTerrain.h>


#include "RenderEngine.h"
#include "EngineUtils.h"
#include "Viewport.h"



void*
CEngineUtils::getBitmapMaterial(const std::string& matName, const std::string& groupName, uint& width, uint& height, uchar& bpp)
{
  try
  {
	  Ogre::Image img;
	  img.load(matName, groupName);

    width = img.getWidth();
    height = img.getHeight();
    bpp = img.getBPP();

    uint totalSize = width * height * bpp / 8;
    void* data = new char[totalSize];
    memcpy(data, img.getData(), totalSize);

    return data;
  }
  catch (Ogre::Exception ex)
  {
    TRACE(ex.getFullDescription().c_str());
  }
  
  return NULL;

	//if (width != -1 && height != -1)
 //   if (img.getFormat() <= Ogre::PF_DXT1 || img.getFormat() >= Ogre::PF_DXT5)
	//	  img.resize(width, height);
	
 // bool ab = cimg.Create(img.getWidth(), img.getHeight(), img.getBPP());

	//for (size_t i = 0; i < img.getWidth(); i++)
	//{
	//	for (size_t j = 0; j < img.getHeight(); j++)
	//	{
	//		Ogre::ColourValue c = img.getColourAt(i, j, 0);
	//		cimg.SetPixelRGB(i, j, c.r * 255, c.g * 255, c.b * 255);
	//	}
	//}

	//return true;
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#if 0
CMeshDecal::CMeshDecal(const std::string& materialName)
{
	m_pManualObj = GetRenderEngine()->getSceneManager()->createManualObject();
	m_pManualObj->setVisibilityFlags(1);

	m_pNode = GetRenderEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
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
	GetRenderEngine()->getSceneManager()->destroySceneNode(m_pNode);
	GetRenderEngine()->getSceneManager()->destroyManualObject(m_pManualObj);
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

	s_vertexDatas** pVerticesDatas = NEW s_vertexDatas*[iSize];

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
				
				fHeight = CTerrainManager::getInstance()->getTerrainHeight(fX, fZ) + 1;
				vecPos = Ogre::Vector3(fX, fHeight, fZ);
				vecTexPos = Ogre::Vector2((fX - fXStart) / fXLength, (fZ - fZStart) / fZLength);
			}

			pVerticesDatas[it] = NEW s_vertexDatas(vecPos, vecTexPos, fHeight);

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

	terrainHeight = NEW float*[terrainSize];

	for (int i = 0; i < terrainSize; i++)
		terrainHeight[i] = NEW float[terrainSize];

	srand((unsigned int)time(0));
	
	for (int j = 0; j < terrainSize; j++)
		for (int i = 0; i < terrainSize; i++)
			terrainHeight[i][j] = -1;

	
	for (int j = 0; j < terrainSize; j++)
		for (int i = 0; i < terrainSize; i++)
			terrainHeight[i][j] = averageHeight((float**)terrainHeight, i, j, terrainSize) + ((float)(std::rand() % 100) / 100) - 0.5f;

	
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
#endif // !0



////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <OgreHighLevelGpuProgramManager.h>
using namespace Ogre;

void UnloadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName)
{
    Ogre::ResourcePtr rPtr = resMgr->getByName(resourceName);
    if (rPtr.isNull())
    {
        StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->unload();
    if (rPtr->isLoaded())
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was not unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    resMgr->remove(resourceName);
    rPtr = resMgr->getByName(resourceName);
    if (rPtr.isNull())
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was not removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (resMgr->resourceExists(resourceName))
    {
        StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}
 
void LoadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName, const std::string& resourceGroup)
{
    Ogre::ResourcePtr rPtr = resMgr->load(resourceName, resourceGroup);
    if (rPtr.isNull())
    {
        StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->reload();
    if (rPtr->isLoaded())
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        StringUtil::StrStreamType msg;
        msg << "Resource was not reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (resMgr->resourceExists(resourceName))
    {
        StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}
 
void UnloadMaterials(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                StringUtil::trim(line);
 
                ///
                /// UNLOAD MATERIALS
                ///
                if (StringUtil::startsWith(line, "material"))
                {
                    Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t:");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::MaterialManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}
 
void UnloadVertexPrograms(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                StringUtil::trim(line);
 
                ///
                /// UNLOAD VERTEX PROGRAMS
                ///
                if (StringUtil::startsWith(line, "vertex_program") && !StringUtil::startsWith(line, "vertex_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}
 
void UnloadFragmentPrograms(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                StringUtil::trim(line);
 
                ///
                /// UNLOAD FRAGMENT PROGRAMS
                ///
                if (StringUtil::startsWith(line, "fragment_program") && !StringUtil::startsWith(line, "fragment_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}
 
void
CEngineUtils::ReloadMaterial(const std::string& materialName, const std::string& groupName, const std::string& filename, bool parseMaterialScript)
{
    if (materialName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material name is empty.");
        return;
    }
 
    if (groupName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Group name is empty.");
        return;
    }
 
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    UnloadMaterials(filename);
    //UnloadVertexPrograms(filename);
    //UnloadFragmentPrograms(filename);
 
    if (parseMaterialScript)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
                Ogre::MaterialManager::getSingleton().parseScript(stream, groupName);
                Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
                if (!materialPtr.isNull())
                {
                    materialPtr->compile();
                    materialPtr->load();
                }
            }
            catch (Ogre::Exception &e)
            {
                StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        ///
        /// RELOAD MATERIAL SCRIPT CONTENTS
        ///
        stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
                ///
                /// RELOAD ALL MATERIAL CONTENTS IN FILE
                ///
                while(!stream->eof())
                {
                    std::string line = stream->getLine();
                    StringUtil::trim(line);
                    ///
                    /// RELOAD MATERIALS
                    ///
                    if (StringUtil::startsWith(line, "material"))
                    {
                        Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t:");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            StringUtil::trim(match);
                            if (!match.empty())
                            {
                                LoadResource(Ogre::MaterialManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                    ///
                    /// RELOAD VERTEX PROGRAMS
                    ///
                    if (StringUtil::startsWith(line, "vertex_program") && !StringUtil::startsWith(line, "vertex_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            StringUtil::trim(match);
                            if (!match.empty())
                            {
															Ogre::GpuProgramPtr program = static_cast<Ogre::GpuProgramPtr>(Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName(match, groupName));
															program->setParameter("compile_arguments", "-DENABLE_XX_FEATURE");
															program->escalateLoading();
															program->reload();

                                //LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                    ///
                    /// RELOAD FRAGMENT PROGRAMS
                    ///
                    if (StringUtil::startsWith(line, "fragment_program") && !StringUtil::startsWith(line, "fragment_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = StringUtil::split(line," \t");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            StringUtil::trim(match);
                            if (!match.empty())
                            {
															Ogre::GpuProgramPtr program = static_cast<Ogre::GpuProgramPtr>(Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName(match, groupName));
															program->setParameter("compile_arguments", "-DENABLE_XX_FEATURE");
															program->escalateLoading();
															program->reload();
															
															//LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                }
            }
            catch (Ogre::Exception &e)
            {
                StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
            return;
        }
        catch (Ogre::Exception &e)
        {
            UnloadVertexPrograms(filename);
 
            StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading vertex programs." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
        }
        catch (Ogre::Exception &e)
        {
            // Don't load the script this time
            ReloadMaterial(materialName, groupName, filename, false);
 
            StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading materials." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
}



////////////////////////////////////////////////////////////////////////////////

#include <OgreMovableObject.h>
#include <OgreMaterialManager.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreBillBoardChain.h>
#include <OgreBillBoardSet.h>
#include <OgreOverlayElement.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

class matReloadVisitor : public Ogre::Renderable::Visitor
{
private:
  matReloadVisitor(const matReloadVisitor&);
  matReloadVisitor operator=(const matReloadVisitor&);
public: 
  /// \brief default constructor
	matReloadVisitor() {};
  /// \brief called for each renderable
  virtual void visit(Ogre::Renderable *rend, Ogre::ushort lodIndex, bool isDebug, Ogre::Any *pAny=0)
	{
		const Ogre::MaterialPtr mat = rend->getMaterial();
		if(!mat.isNull())
		{
			std::string newMatName = mat->getName();
			Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(newMatName);
			if(newMat.isNull())
			{
				// this can happen if there was error during the reloading of the material.
				// in that case, we keep the ancient one.
				// Ogre::LogManager::getSingleton().logMessage(newMatName+" : NEW material is null!");
				return;
			}

			// unfortunately, the renderable gives access only to a const MaterialPtr.
			// and there is no 'setMaterial' or 'setMaterialName' method on renderables.
			// so I have to try to down cast with known classes...
			{   
				Ogre::SubEntity* lRend = dynamic_cast<Ogre::SubEntity*>(rend);
				if (lRend){lRend->setMaterialName(newMatName);return;} 
			}
			{
				Ogre::SimpleRenderable* lRend = dynamic_cast<Ogre::SimpleRenderable*>(rend);
				if (lRend){lRend->setMaterial(newMatName);return;} 
			}
			{
				Ogre::ShadowRenderable* lRend = dynamic_cast<Ogre::ShadowRenderable*>(rend);
				if (lRend){lRend->setMaterial(newMat);return;} 
			}
			{   
				Ogre::BillboardChain* lRend = dynamic_cast<Ogre::BillboardChain*>(rend);
				if (lRend){lRend->setMaterialName(newMatName);return;} 
			}
			{   
				Ogre::BillboardSet* lRend = dynamic_cast<Ogre::BillboardSet*>(rend);
				if (lRend){lRend->setMaterialName(newMatName);return;} 
			}
			{   
				Ogre::OverlayElement* lRend = dynamic_cast<Ogre::OverlayElement*>(rend);
				if (lRend){lRend->setMaterialName(newMatName);return;} 
			}
		}else{
			// was there for debug...
			// Ogre::LogManager::getSingleton().logMessage("material of renderable is null!");
		}
	}
};

void
CEngineUtils::updateOnEveryRenderable()
{
   // 1/ get all the available object type (entity, light, user defined types ...)
   std::vector<std::string> allAvailableTypes;
   Ogre::Root::MovableObjectFactoryIterator iterFactory = Ogre::Root::getSingleton().getMovableObjectFactoryIterator();

   for(; iterFactory.hasMoreElements() ;)
   {
      Ogre::MovableObjectFactory* factory = iterFactory.getNext();
      allAvailableTypes.push_back(factory->getType());
   }

   matReloadVisitor visitor;

   // 2/ for each scene manager, lets visit renderables!
   // unfortunately that does not cover all renderables type... (overlays...)
   Ogre::SceneManagerEnumerator::SceneManagerIterator iterSceneManager = Ogre::Root::getSingleton().getSceneManagerIterator();

   for(; iterSceneManager.hasMoreElements() ;)
   {
      Ogre::SceneManager * scMgr = iterSceneManager.getNext();

      std::vector<std::string>::iterator iterMovableType = allAvailableTypes.begin();
      std::vector<std::string>::iterator iterMovableTypeEnd = allAvailableTypes.end();

      for(; iterMovableType!=iterMovableTypeEnd; iterMovableType++)
      {
         Ogre::SceneManager::MovableObjectIterator iterMovable = scMgr->getMovableObjectIterator(*iterMovableType);
         for(; iterMovable.hasMoreElements() ;)
         {
            Ogre::MovableObject * movable = iterMovable.getNext();
						movable->visitRenderables(&visitor, false);
         }
      }
   }

   // 3 / visit overlays!
   {
      Ogre::OverlayManager::OverlayMapIterator iterOverlay = Ogre::OverlayManager::getSingleton().getOverlayIterator();
      for(;iterOverlay.hasMoreElements();)
      {
         Ogre::Overlay* lOverlay = iterOverlay.getNext();
         // get the first level of OverlayContainer in the Overlay
         Ogre::Overlay::Overlay2DElementsIterator iterOverlayElem = lOverlay->get2DElementsIterator();
         for(;iterOverlayElem.hasMoreElements();)
         {
            Ogre::OverlayContainer * lOverlayCont = iterOverlayElem.getNext();
						lOverlayCont->visitRenderables(&visitor, false);
         }
      }
   }
}