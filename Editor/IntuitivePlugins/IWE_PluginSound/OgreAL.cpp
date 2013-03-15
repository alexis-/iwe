#include "IWEPlugin.h"

#include <OgreRoot.h>
#include <OgreOggSoundManager.h>
#include <OgreOggISound.h>

#include "OgreAL.h"


COgreAL*          COgreAL::m_pInst = NULL;

COgreAL*
COgreAL::getInstance()
{
  if (!m_pInst)
    m_pInst = new COgreAL();

  return m_pInst;
}

COgreAL::COgreAL()
  : m_pOgreSound(NULL),
    m_pNodeListener(NULL)
{
}

COgreAL::~COgreAL()
{
  m_pOgreSound->destroyAllSounds();
}



static const char*
getUniqueName(const std::string& name)
{
	static std::map<std::string, unsigned long>		nameMap;
	static char																		pNameRet[2048];
	std::string																		sName(name);
	unsigned long																	id = 0;

	if (nameMap.find(sName) != nameMap.end())
	{
		id = ++nameMap[sName];
		nameMap[sName] = id;
	}
	else
		nameMap[sName] = id;

	sName += "." + Ogre::StringConverter::toString(id);
	strncpy(pNameRet, sName.c_str(), 2048);

	return pNameRet;
}

void
COgreAL::init()
{
  
#ifdef _DEBUG
  GetRenderEngine()->getRoot()->loadPlugin("OgreOggSound_d");
#else
  GetRenderEngine()->getRoot()->loadPlugin("OgreOggSound");
#endif // !_DEBUG

  m_pOgreSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr();

  m_pOgreSound->init("", 100U, 64U, GetRenderEngine()->getSceneManager());
  m_pOgreSound->setDistanceModel(AL_LINEAR_DISTANCE);

  //Ogre::Camera* pCamera = (*(GetRenderEngine()->getSceneManager()->getCameras().begin())).second;
  //pCamera->getParentSceneNode()->attachObject(m_pOgreSound->getListener());
  m_pNodeCam = GetRenderEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode(getUniqueName("OgreOggListener"));
  //m_pNodeCam = m_pNodeListener->createChildSceneNode("pCamNode");

  //m_pNodeCam->attachObject(pCamera);
  //m_pNodeCam = pCamera->getParentSceneNode();
  m_pNodeCam->attachObject(m_pOgreSound->getListener());
  
  GetEditorInteraction()->eventCameraMoved +=
    FastDelegate1<ICameraController*>(this, &COgreAL::onCameraMoved);
  GetEditorInteraction()->eventCameraRotated +=
    FastDelegate1<ICameraController*>(this, &COgreAL::onCameraRotated);
}

OgreOggSound::OgreOggISound*
COgreAL::create(const std::string& sSoundFile, bool bStream, bool bLoop, bool bPreBuffer)
{
  return create(getUniqueName(sSoundFile), sSoundFile, bStream, bLoop, bPreBuffer);
}

OgreOggSound::OgreOggISound*
COgreAL::create(const std::string& sName, const std::string& sSoundFile, bool bStream, bool bLoop, bool bPreBuffer)
{
  return m_pOgreSound->createSound(sName, sSoundFile, bStream, bLoop, bPreBuffer, GetRenderEngine()->getSceneManager());
}

void
COgreAL::destroySound(OgreOggSound::OgreOggISound* pOgreSound)
{
  pOgreSound->stop(true);
  m_pOgreSound->destroySound(pOgreSound);
}

void
COgreAL::update(float fDelta)
{
  m_pOgreSound->update(fDelta);
}

void
COgreAL::onCameraMoved(ICameraController* pCameraController)
{
  m_pNodeCam->setPosition(pCameraController->getViewport()->getCamera()->getPosition());
  //m_pOgreSound->getListener()->setPosition(pCameraController->getViewport()->getCamera()->getPosition());
}

void
COgreAL::onCameraRotated(ICameraController* pCameraController)
{
  m_pNodeCam->setOrientation(pCameraController->getViewport()->getCamera()->getOrientation());
  //m_pOgreSound->getListener()->setOrientation(pCameraController->getViewport()->getCamera()->getOrientation());
}