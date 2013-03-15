#ifndef ENGINE_UTILS_H
# define ENGINE_UTILS_H


class CViewport;

class CEngineUtils
{
public:
  

	static Ogre::Vector3					GetPlaneCoord(float x, float y, CViewport* pViewport, const Ogre::Vector3& vecNormal, const Ogre::Vector3& vecPos);
	static Ogre::Vector3					GetHeightCoord(float x, float y, CViewport* pViewport, const float fHeight);

	static void*									getBitmapMaterial(const std::string& matName, const std::string& groupName, uint& width, uint& height, uchar& bpp);

  static void                   ReloadMaterial(const std::string& materialName, const std::string& groupName, const std::string& filename, bool parseMaterialScript);
	static void										updateOnEveryRenderable();
};

#if 0
class CMeshDecal
{
public:
	//CMeshDecal(float x, float z, float rad, const std::string& materialName);
	CMeshDecal(const std::string& materialName);
	~CMeshDecal();

	virtual void update(float x, float z, float rad);
	virtual void update(float x, float z, float rad, const std::string& materialName);

	virtual void setVisible(bool bVisible);

protected:
	Ogre::ManualObject*			m_pManualObj;
	Ogre::SceneNode*				m_pNode;
};

class CTestMesh : public CMeshDecal
{
public:
	CTestMesh(const std::string& materialName)
		: CMeshDecal(materialName)
	{
	};
	
	virtual void update(float x, float z, float rad);
};
#endif



#endif // !ENGINE_UTILS_H