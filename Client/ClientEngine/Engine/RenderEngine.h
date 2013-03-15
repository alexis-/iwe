#ifndef RENDERENGINE_H
# define RENDERENGINE_H

# include "iwebase.h"

# include <vector>

# include <OgreCamera.h>
# include <OgreEntity.h>
# include <OgreLogManager.h>
# include <OgreOverlay.h>
# include <OgreOverlayElement.h>
# include <OgreOverlayManager.h>
# include <OgreRoot.h>
# include <OgreViewport.h>
# include <OgreSceneManager.h>
# include <OgreRenderWindow.h>
# include <OgreOverlayElement.h>
# include <OgreConfigFile.h>
# include <OgreMaterialManager.h>

# include "Viewport.h"

class COgreLogListener;

class CRenderEngine : public IRenderEngine,
											public Ogre::Singleton<CRenderEngine>
{
public:
	CRenderEngine();
	virtual ~CRenderEngine();

  static CRenderEngine*	                          getInstance();

	void																            logMessage(const char*	fmt, ...);
	virtual bool																    init();

public:
	Ogre::Camera*																    createCamera(void);

	virtual IViewport*													    createViewport(size_t handle, const CIWEPoint<ushort>& size, bool bEditorViewport = true, int maskID = 1);
	virtual const std::vector<IViewport*>&			    getViewports();

	virtual void																    render();
  virtual const std::vector<IWEVariantSetting>&   getRenderSettings();
  virtual void                                    setRenderSetting(const IWEVariantSetting& setting);
  
  virtual void                                    customCmd(int nArgs, ...);

private:
	bool																				    setupResources(void);
  void                                            setupRenderSettings();

  void                                            onResourceUpdated();

public:
	virtual Ogre::SceneManager*									    getSceneManager()
	{
		return m_pSceneMgr;
	}
	
	virtual Ogre::Root*													    getRoot()
	{
		return m_pRoot;
	}

private:
  Ogre::Root*																	    m_pRoot;
  Ogre::Camera*																    m_pCamera;
  Ogre::SceneManager*													    m_pSceneMgr;
	Ogre::Timer																	    m_FrameTimer;

	COgreLogListener*														    m_pLogListener;

	std::vector<IViewport*>											    m_pViewports;

  std::vector<IWEVariantSetting>                  m_vecSettings;
};

CRenderEngine*																    GetRenderEngine();

#endif // !RENDERENGINE_H