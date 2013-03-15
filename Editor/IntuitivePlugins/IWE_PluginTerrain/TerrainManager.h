#ifndef TERRAIN_MANAGER_H
# define TERRAIN_MANAGER_H

# include <Terrain\OgreTerrain.h>
# include <Terrain\OgreTerrainGroup.h>

struct										s_terrainCreationInfo
{
	int											iX;
	int											iY;
	Ogre::Vector3						vecPos;
	Ogre::TerrainGroup*			pTerrainGroup;
};


namespace Ogre
{
	class TerrainFix : public Terrain
	{
	public:
		void finishDerivedWork();
	};
}

class CTerrainManager
{
public:
	static CTerrainManager*														getInstance();
	~CTerrainManager();

public:
	bool																							terrainIsDefined(long x, long y);

	UINT																							createTerrainGroup(float worldSize);
	bool																							checkTerrainIntersect(Ogre::RealRect rect);
	bool																							isValidGroupID(UINT nGroupID);
	bool																							createTerrain(long iX, long iY, UINT nGroupID, const Ogre::Vector3& terrainPosition);
	bool																							createTerrain(long iX, long iY, UINT nGroupID);
	bool																							isValidTerrainSpot(int iX, int iY, const Ogre::RealRect& rect);
	float																							getGroupSize(UINT nGroupID);
	void																							initBlendMaps(Ogre::Terrain* terrain);

	void																							clear();
	void																							addTerrain(Ogre::Terrain* pTerrain, int iX, int iY);

	float																							getTerrainHeight(float x, float z);
	bool																							getTerrainCoord(float x, float y, IViewport* pViewport, Ogre::Vector3& vecOut);
	bool																							getTerrainCoord(const Ogre::Ray& ray, const Ogre::Vector3& vecCamPos, Ogre::Vector3& vecOut);

	bool																							getTerrainDatas(float x, float y, IViewport* pViewport, Ogre::Vector3& vecOut, Ogre::Terrain*& pTerrainOut);

	void																							setDecalVisible(bool bShow);
	void																							setDecalPosition(Ogre::Vector2 vecPos);
	void																							setDecalPosition(Ogre::Vector3 vecPos);
	void																							setDecalTexture(std::string sTexName);
	void																							setDecalSize(float size);

public:
	inline const std::vector<Ogre::TerrainGroup*>&		getTerrainGroups() const
	{
		return m_vecTerrainGroups;
	}

	inline const std::vector<Ogre::Terrain*>&					getTerrains() const
	{
		return m_vecTerrains;
	}

	inline const std::vector<Ogre::RealRect>&					getTerrainBounds() const
	{
		return m_vecTerrainBounds;
	}

	inline Ogre::Frustum*															getDecalFrustum() const
	{
		return m_pDecalFrustum;
	}

	inline Ogre::SceneNode*														getDecalNode() const
	{
		return m_pDecalNode;
	}

	inline Ogre::TexturePtr														getDecalTexture() const
	{
		return m_pDecalTexture;
	}

	inline Ogre::Image																getDecalImage() const
	{
		return m_decalImg;
	}

private:

private:
	CTerrainManager();
	static CTerrainManager*														m_pInst;
  
  Ogre::TerrainGlobalOptions*									      m_pTerrainGlobals;
	
	std::map<std::pair<long,long>, Ogre::Terrain*>		m_mapTerrains;
	std::vector<Ogre::TerrainGroup*>									m_vecTerrainGroups;
	std::vector<Ogre::RealRect>												m_vecTerrainBounds;
	std::vector<Ogre::Terrain*>												m_vecTerrains;
	std::vector<s_terrainCreationInfo>								m_vecPendingTerrains;

	Ogre::Frustum*																		m_pDecalFrustum;
	Ogre::SceneNode*																	m_pDecalNode;
	Ogre::TexturePtr																	m_pDecalTexture;
	Ogre::Image																				m_decalImg;

	Ogre::Terrain*																		m_pLastTerrain;
	Ogre::RealRect																		m_lastTerrainBounds;

public:
  static int                                        m_iTerrainSize;
  static float                                      m_fTerrainWorldSize;
};

#endif // !TERRAIN_MANAGER_H