#ifndef ENV_MAP_GENERATOR_H
# define ENV_MAP_GENERATOR_H

# include <OgreRenderTargetListener.h>
# include <OgreCamera.h>

class CEnvMapGenerator : public Ogre::RenderTargetListener
{
public:
  CEnvMapGenerator();
  ~CEnvMapGenerator();

public:
  void                                                initialize(Ogre::SceneManager* pSceneMgr, Ogre::Viewport* pMainViewport);

private:
  std::string                                         m_sTexName;
  Ogre::Camera*                                       m_pCamera;
  
	Ogre::RenderTarget*                                 m_pTargets[6];

  int                                                 m_iCount;

  int                                                 howMany()
  {
    return m_iCount++;
  }

};

#endif // !ENV_MAP_GENERATOR_H