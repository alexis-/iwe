#include "stdafx.h"

#include "RenderEngine.h"
#include "ResourceManager.h"
#include "Viewport.h"


class COgreLogListener : public Ogre::LogListener
{
public:
	COgreLogListener()
	{
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(this);
	}

	virtual void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool&)
	{
		//COutputWnd* pOutput = CMainFrame::getInstance()->getOutputWindow(); // TODO: Notify GUI

		//if (!pOutput)
		//	m_vecPendingMsg.push_back(message);
		//else
		//{
		//	for (int i = 0; i < m_vecPendingMsg.size(); i++)
		//		SendMessage(m_vecPendingMsg[i]);
    //
		//	m_vecPendingMsg.clear();
    //
    //  SendMessage(message);
		//}
	}

private:
  void SendMessage(const Ogre::String& msg)
  {
    //char* pMsg = NEW char[msg.size() + 1];
    //
    //strcpy(pMsg, msg.c_str());
    //pMsg[msg.size()];
    //
    //::PostMessage(CMainFrame::getInstance()->GetSafeHwnd(), WM_OUTPUT_ADD, 0, (LPARAM)pMsg);
  }

private:
	std::vector<std::string> m_vecPendingMsg;
};

bool
CRenderEngine::init()
{
	try
	{
		m_pRoot = OGRE_NEW Ogre::Root("", "", "IntuitiveEditor.log");

		m_pLogListener = NEW COgreLogListener();
	
#ifdef _DEBUG
		m_pRoot->loadPlugin("RenderSystem_Direct3D9_d");
    //m_pRoot->loadPlugin("RenderSystem_Direct3D11_d");
		//m_pRoot->loadPlugin("RenderSystem_GL_d");

		m_pRoot->loadPlugin("Plugin_CgProgramManager_d");
		m_pRoot->loadPlugin("Plugin_OctreeSceneManager_d");

    //m_pRoot->loadPlugin("OgreOggSound_d");

#else
		m_pRoot->loadPlugin("RenderSystem_Direct3D9");
    //m_pRoot->loadPlugin("RenderSystem_Direct3D11");
		//m_pRoot->loadPlugin("RenderSystem_GL");

		m_pRoot->loadPlugin("Plugin_CgProgramManager");
		m_pRoot->loadPlugin("Plugin_OctreeSceneManager");

    //m_pRoot->loadPlugin("OgreOggSound");
#endif // !_DEBUG

		Ogre::RenderSystem* pSystem = m_pRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
		//Ogre::RenderSystem* pSystem = m_pRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

		pSystem->setConfigOption("Full Screen", "No");
		pSystem->setConfigOption("VSync", "Yes");

		m_pRoot->setRenderSystem(pSystem);

		setupResources();

		m_pRoot->initialise(false);


    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC);

		m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

		m_FrameTimer.reset();

		return true;
	}
	catch (Ogre::Exception e)
	{
		logMessage(e.getFullDescription().c_str());

		return false;
	}
}

 // TODO: Move this away ?
bool
CRenderEngine::setupResources(void)
{
    Ogre::ConfigFile		cf;
    Ogre::String				secName,
												typeName,
												archName;

	try
	{
		// Load resource paths from config file
		cf.load("resources.cfg");
	}
	catch (Ogre::FileNotFoundException	e)
	{
		logMessage(e.getFullDescription().c_str());

		return false;
	}

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  while (seci.hasMoreElements())
  {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
          typeName = i->first;
          archName = i->second;

          Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
              archName, typeName, secName);
      }
  }

	return true;
}

Ogre::Camera*
CRenderEngine::createCamera(void)
{
  // Create the camera
  Ogre::SceneNode* pCameraNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(getUniqueName("EditorCamSceneNode"));
  m_pCamera = m_pSceneMgr->createCamera(getUniqueName("EditorCam"));
  pCameraNode->attachObject(m_pCamera);

  m_pCamera->setNearClipDistance(1);
	m_pCamera->setFarClipDistance(1000);
	
	//if (m_pRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
 // {
 //     m_pCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
 // }

	Ogre::Vector3 pos(-20.0f, 20.0f, -20.0f);
	m_pCamera->setPosition(pos);
	m_pCamera->lookAt(Ogre::Vector3::ZERO);

	return m_pCamera;
}

IViewport*
CRenderEngine::createViewport(size_t handle, const CIWEPoint<ushort>& size, bool bEditorViewport, int maskID)
{
	try
	{
		CViewport* pViewport = NEW CViewport(handle, size, bEditorViewport, maskID);
		m_pViewports.push_back(pViewport);

    eventViewportCreated(pViewport);
		
		return pViewport;
	}
	catch (Ogre::Exception e)
	{
		logMessage(e.getFullDescription().c_str());

		return NULL;
	}
}

void
CRenderEngine::setupRenderSettings()
{
}

const std::vector<IWEVariantSetting>&
CRenderEngine::getRenderSettings()
{
  return m_vecSettings;
}

void
CRenderEngine::setRenderSetting(const IWEVariantSetting& setting)
{
}