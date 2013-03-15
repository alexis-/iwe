#ifndef TEST_MESH_H
# define TEST_MESH_H

namespace Ogre
{
  class SceneManager;
  class ManualObject;
  class SceneNode;
};

class IWEAPI CMeshDecal
{
public:
	//CMeshDecal(float x, float z, float rad, const std::string& materialName);
	CMeshDecal(const std::string& materialName, Ogre::SceneManager* pSceneMgr);
	~CMeshDecal();

	virtual void update(float x, float z, float rad);
	virtual void update(float x, float z, float rad, const std::string& materialName);

	virtual void setVisible(bool bVisible);

protected:
  Ogre::SceneManager*     m_pSceneMgr;

	Ogre::ManualObject*			m_pManualObj;
	Ogre::SceneNode*				m_pNode;
};

class IWEAPI CTestMesh : public CMeshDecal
{
public:
	CTestMesh(const std::string& materialName, Ogre::SceneManager* pSceneMgr)
		: CMeshDecal(materialName, pSceneMgr)
	{
	};
	
	virtual void update(float x, float z, float rad);
};

#endif // !TEST_MESH_H